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
	m_keySent(false) , m_keyAccepted(false) , m_gotRSAPub(false)
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
	if(!ControlCenter::instance()->cryptEngine()->encryptAES(m_aesPassPhrase.toUtf8(),data,encData))
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
		QByteArray resultTemp = readAESDecryptedData();
		if(resultTemp.isEmpty())
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

	QByteArray result = readAESDecryptedData();

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

	if(!m_keyAccepted)
		getRSAPub();
	else
		getAESPass();

	if(!m_keyAccepted || !m_keySent || secure())
		return;
	sendAESPass();
}

void SecureTunnel::getRSAPub()
{
	Message m = Message::fromJson(readRawData());
	if(m.method() != Message::SECURE)
		return;

	if(m.option() == Message::RSA)
	{
		m_clientPublicKey = m.data("KEY").toUtf8();
		if(m_clientPublicKey.isEmpty())
			return;
		m_keyAccepted = true;

		if(!m_keySent)
			sendKey();
	}
}

void SecureTunnel::getAESPass()
{
	if(!m_keySent || !m_keyAccepted)
		return;

	Message m = Message::fromJson(readRSADecryptedData());
	if(m.method() != Message::SECURE)
		return;

	if(m.option() == Message::AES)
	{
		if(m_aesPassPhrase.isEmpty()) {
			m_aesPassPhrase = m.data("KEY");
		}
		else if(m_aesPassPhrase < m.data("KEY").toUtf8()) {
			m_aesPassPhrase = m.data("KEY");
		}
		else {
			m_secured = true;
			disconnect(m_conn,&IConnection::dataAvailable,this,&SecureTunnel::readyRead);
			emit secured();
			return;
		}

		if(m_aesPassPhrase.isEmpty())
			return;

		m_secured = true;
		disconnect(m_conn,&IConnection::dataAvailable,this,&SecureTunnel::readyRead);
		emit secured();
	}
}

void SecureTunnel::sendAESPass()
{
	if(!m_keySent || !m_keyAccepted || secure())
		return;

	QByteArray passphrase;
	if(!m_aesPassPhrase.isEmpty())
	{
		passphrase = ControlCenter::instance()->cryptEngine()->randomPassword();
		if(passphrase < m_aesPassPhrase)
			return;
		m_aesPassPhrase = passphrase;
	}
	else
	{
		m_aesPassPhrase = ControlCenter::instance()->cryptEngine()->randomPassword();
	}

	Message m;
	m.setMethod(Message::SECURE);
	m.setOption(Message::AES);
	m.setData("KEY",m_aesPassPhrase);

	QByteArray data = m.toJson();
	QByteArray encData;
	if(!ControlCenter::instance()->cryptEngine()->encryptRSA(m_clientPublicKey,data,encData))
		return;

	encData = dataSize(encData.size()) + encData;

	m_conn->write(encData);
	m_conn->flush();
	m_conn->waitForBytesWritten();
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

QByteArray SecureTunnel::readAESDecryptedData()
{
	QByteArray data = readRawData();
	QByteArray result;
	if(data.isEmpty())
		return result;
	ControlCenter::instance()->cryptEngine()->decryptAES(m_aesPassPhrase.toUtf8(),data,result);
	return result;
}

QByteArray SecureTunnel::readRSADecryptedData()
{
	QByteArray data = readRawData();
	QByteArray result;
	if(data.isEmpty())
		return result;
	ControlCenter::instance()->cryptEngine()->decryptRSA(data,result);
	return result;
}

QByteArray SecureTunnel::readRawData()
{
	QByteArray data;
	QByteArray l = m_conn->seek(8);

	if(l.isEmpty())
		return data;

	quint32 size = l.toULong();

	if(m_conn->availableSize() < size + 8)
		return data;

	data = m_conn->data(size+8).mid(8);
	return data;
}

}
