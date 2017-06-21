/*
 *   This file is part of IPConnect
 *   Copyright 2017 Shashwat Dixit <shashwatdixit124@gmail.com>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU Library General Public License as
 *   published by the Free Software Foundation; either version 3 of the
 *   License, or (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU Library General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include "securetunnel.h"

#include "interfaces/iconnection.h"
#include "interfaces/icryptengine.h"
#include "controlcenter.h"
#include "message.h"

#include <QObject>

namespace IPConnect
{

SecureTunnel::SecureTunnel(QObject* parent) : QObject(parent) , m_secured(false) , m_conn(nullptr) ,
	m_keySent(false) , m_keyAccepted(false)
{
}

SecureTunnel::~SecureTunnel()
{
}

void SecureTunnel::create()
{
	if(!m_conn)
		return;
	sendKey();
}

bool SecureTunnel::secure() const
{
	return m_secured;
}

void SecureTunnel::send(QByteArray data)
{
	if(!secure())
		return;
	QByteArray encData;
	if(!ControlCenter::instance()->cryptEngine()->encryptData(m_clientPublicKey,data,encData))
		return;

	encData = dataSize(encData.size()) + encData;

	m_conn->write(encData);
	m_conn->flush();
	m_conn->waitForBytesWritten();
}

QByteArray SecureTunnel::read()
{
	if(!secure())
		return QByteArray();

	QByteArray result;
	while(true)
	{
		QByteArray l = m_conn->seek(8);

		if(l.isEmpty())
			break;

		quint32 size = l.toULong();

		if(m_conn->availableSize() < size + 8)
			break;

		QByteArray data = m_conn->data(size+8).mid(8);
		QByteArray resultTemp;
		if(!ControlCenter::instance()->cryptEngine()->decryptData(data,resultTemp))
			break;
		result += resultTemp;
	}

	return result;
}

Message SecureTunnel::readMessage()
{
	Message m;
	if(!secure())
		return m;

	QByteArray l = m_conn->seek(8);

	if(l.isEmpty())
		return m;

	quint32 size = l.toULong();

	if(m_conn->availableSize() < size + 8)
		return m;

	QByteArray data = m_conn->data(size+8).mid(8);

	QByteArray result;
	if(!ControlCenter::instance()->cryptEngine()->decryptData(data,result))
		return m;

	m = Message::fromJson(result);
	return m;
}

void SecureTunnel::setConnection(IConnection* conn)
{
	m_conn = conn;
	connect(m_conn,&IConnection::dataAvailable,this,&SecureTunnel::readyRead);
}

void SecureTunnel::readyRead()
{
	if(secure())
		return;

	QByteArray l = m_conn->seek(8);

	if(l.isEmpty())
		return;

	quint32 size = l.toULong();

	if(m_conn->availableSize() < size + 8)
		return;

	QByteArray data = m_conn->data(size+8).mid(8);

	Message m = Message::fromJson(data);

	if(m.method() == Message::SECURE && m.option() == Message::RSA)
	{
		m_clientPublicKey = m.data("KEY").toUtf8();
		if(m_clientPublicKey.isEmpty())
			return;
		m_keyAccepted = true;

		if(!m_keySent)
			sendKey();
		if(m_keySent && m_keyAccepted)
		{
			m_secured = true;
			disconnect(m_conn,&IConnection::dataAvailable,this,&SecureTunnel::readyRead);
			emit secured();
		}
	}
}

void SecureTunnel::sendKey()
{
	if(m_keySent)
		return;

	m_keySent = true;
	Message m;
	m.setMethod(Message::SECURE);
	m.setOption(Message::RSA);
	m.setData("KEY",ControlCenter::instance()->cryptEngine()->publicKey());
	QByteArray msg = m.toJson();
	msg = dataSize(msg.size()) + msg;
	m_conn->write(msg);
	m_conn->flush();
	m_conn->waitForBytesWritten();
}

QByteArray SecureTunnel::dataSize(quint32 size)
{
	QString len = QString::number(size);
	for(int i = len.size() + 1;i<=8;i++)
	{
		len = QStringLiteral("0")+len;
	}
	return len.toUtf8();
}

}
