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

#include "client.h"

#include "interfaces/iclient.h"
#include "interfaces/iconnection.h"
#include "interfaces/icryptengine.h"
#include "interfaces/iusersettings.h"
#include "controlcenter.h"
#include "clientinformation.h"
#include "debug.h"
#include "message.h"

#include <QObject>

namespace IPConnect
{

Client::Client(QObject* parent) : IClient(parent) , m_conn(nullptr) , m_detailAccepted(false) , 
	m_detailSent(false) , m_pubKeySent(false) , m_pubKeyAccepted(false)
{
}

Client::~Client()
{
}

IConnection* Client::connection()
{
	return m_conn;
}

ClientInformation Client::info()
{
	return m_info;
}

void Client::sendMessage(QString msg)
{
	Message m;
	m.setMethod(Message::MSG);
	m.setOption(Message::TEXT);
	m.setData("MSG",msg);
	sendEncrypted(m);
}

void Client::setConnection(IConnection* conn)
{
	m_conn = conn;
	connect(m_conn,&IConnection::dataAvailable,this,&Client::handleRead);
	connect(m_conn,&IConnection::disconnected,this,&Client::closeConnection);
	connect(m_conn,&IConnection::errorOccurred,this,&Client::closeConnection);
}

void Client::setInfo(ClientInformation info)
{
	m_info = info;
}

void Client::start()
{
	sendPublicKey();
}

bool Client::hasAcceptedData() const
{
	return m_detailAccepted;
}

void Client::handleRead()
{
	QByteArray data = m_conn->data();
	processRead(data);
}

void Client::closeConnection()
{
	emit connectionClosed();
}

void Client::processRead(QByteArray t_data)
{
	if(m_pubKeyAccepted && m_pubKeySent) {
	QByteArray dData;
	if(!ControlCenter::instance()->cryptEngine()->decryptData(t_data,dData))
		return;
		m_request = Message::fromJson(dData);
	} else {
		m_request = Message::fromJson(t_data);
	}
	handleRequest();
}

void Client::handleRequest()
{
	if(m_request.method() == Message::SECURE)
	{
		if(m_request.option() != Message::RSA)
			return;

		if(m_pubKeyAccepted)
			return;

		m_clientPublicKey = m_request.data("KEY");

		if(m_clientPublicKey.isEmpty())
			return;
		m_pubKeyAccepted = true;

		if(!m_pubKeySent)
			sendPublicKey();
		else
			sendDetail();
	}
	else if(m_request.method() == Message::CONNECT)
	{
		if(!m_pubKeyAccepted || !m_pubKeySent)
			return;

		if(m_request.option() == Message::REQUEST)
		{
			if(!m_detailAccepted)
			{
				qCDebug(BASE) << this << "Accepting Detail";
				QString ip = m_request.data("IP");
				QString name = m_request.data("NAME");

				if(ip.isEmpty() || name.isEmpty())
					return;

				m_info.setName(name.trimmed());
				m_info.setIp(ip.trimmed());
				qCDebug(BASE) << this << "Accepted request from "<< name;
				emit infoRecieved(m_info);
				m_detailAccepted = true;

				if(!m_detailSent)
					sendDetail();
			}
		}
	}
	else if(m_request.method() == Message::MSG)
    {
		if(!m_pubKeyAccepted || !m_pubKeySent)
			return;

		if(!m_detailSent || !m_detailAccepted)
			return;

		if(m_request.option() == Message::TEXT) emit messageRecieved(m_info.id(),m_request.data("MSG"));
	}
	return;
}

void Client::send(Message m)
{
	if(m_conn){
		m_conn->write(m.toJson());
		m_conn->flush();
		m_conn->waitForBytesWritten();
	}
	else
		qCDebug(BASE) << this << "Connection not available on write";
}

void Client::sendEncrypted(Message m)
{
	if(m_conn){
		QByteArray msg = m.toJson();
		QByteArray encMsg;
		QByteArray pubKey = m_clientPublicKey.toUtf8();
		if(!ControlCenter::instance()->cryptEngine()->encryptData(pubKey,msg,encMsg))
			return;
		m_conn->write(encMsg);
		m_conn->flush();
		m_conn->waitForBytesWritten();
	}
	else
		qCDebug(BASE) << this << "Connection not available on write";
}

void Client::sendDetail()
{
	if(m_conn)
	{
		if(!m_detailSent)
		{
			m_detailSent = true;
			QString myIp = ControlCenter::instance()->userSettings()->ip();
			QString myName = ControlCenter::instance()->userSettings()->name();
			Message m;
			m.setMethod(Message::CONNECT);
			m.setOption(Message::REQUEST);
			m.setData("NAME",myName);
			m.setData("IP",myIp);
			sendEncrypted(m);
		}
	}
	else
		qCDebug(BASE) << this << "Connection not available on greetings";
}

void Client::sendPublicKey()
{
	m_pubKeySent = true;
	Message m;
	m.setMethod(Message::SECURE);
	m.setOption(Message::RSA);
	QByteArray pubKey = ControlCenter::instance()->cryptEngine()->publicKey();
	m.setData("KEY",pubKey);
	send(m);
}

}
