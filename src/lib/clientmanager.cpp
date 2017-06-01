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

#include "clientmanager.h"

#include "interfaces/iclientmanager.h"
#include "interfaces/iconnection.h"
#include "interfaces/iclient.h"
#include "client.h"
#include "connection.h"
#include "clientinformation.h"
#include "clientthreadmanager.h"
#include "debug.h"
#include "messageinformation.h"

#include <QObject>
#include <QList>
#include <QThread>

namespace IPConnect
{

ClientManager::ClientManager(QObject* parent)
	: IClientManager(parent) , m_clientCount(0) , m_messageCount(0) ,
	m_clientThread(new QThread(this)) , m_clientThreadManager(new ClientThreadManager())
{
	qCDebug(BASE) << "ClientManager started";
	connect(m_clientThreadManager,&ClientThreadManager::clientAdded,this,&ClientManager::clientAdded,Qt::QueuedConnection);
	connect(m_clientThreadManager,&ClientThreadManager::clientRemoved,this,&ClientManager::clientRemoved,Qt::QueuedConnection);
	connect(m_clientThreadManager,&ClientThreadManager::messageAdded,this,&ClientManager::addMessage,Qt::QueuedConnection);
	connect(this,&ClientManager::clientCreated,m_clientThreadManager,&ClientThreadManager::clientCreated,Qt::QueuedConnection);
	connect(this,&ClientManager::removeAllClients,m_clientThreadManager,&ClientThreadManager::removeAllClients,Qt::QueuedConnection);
	connect(this,&ClientManager::sendToClient,m_clientThreadManager,&ClientThreadManager::sendToClient,Qt::QueuedConnection);
	m_clientThreadManager->moveToThread(m_clientThread);
	m_clientThread->start();
}

ClientManager::~ClientManager()
{
}

void ClientManager::shutdown()
{
	emit removeAllClients();
	m_clientThread->quit();
	m_clientThread->deleteLater();
	qCDebug(BASE) << "ClientManager Stopped" ;
}

QList<ClientInformation> ClientManager::clients()
{
	return m_clientsInfo.values();
}

ClientInformation ClientManager::clientInfo(qint16 id)
{
	return m_clientsInfo.value(id);
}

QList<MessageInformation> ClientManager::messages()
{
	return m_messages.values();
}

void ClientManager::clientRemoved(qint16 id)
{
	m_clientsInfo.remove(id);
	emit userListUpdated();
}

void ClientManager::addConnection(IConnection* connection)
{
	qCDebug(BASE) << this << "adding new Client with descriptor " <<  connection->socketDescriptor();
	Client* client = createClient(connection);

	if(!client){
		qCDebug(BASE) << "cannot add Client with socket descriptor " << connection->socketDescriptor() ;
		return;
	}

	connection->moveToThread(m_clientThread);
	client->moveToThread(m_clientThread);
	emit clientCreated(client);
}

void ClientManager::sendMessage(qint16 id,QString msg)
{
	MessageInformation mi;
	mi.setSent(true);
	mi.setUser(m_clientsInfo.value(id).name());
	mi.setMsg(msg);
	m_messages.insert(m_messageCount++,mi);
	emit sendToClient(id,msg);
	emit messageAdded(mi);
}

void ClientManager::connectManualy(QString url)
{
	Connection* con = new Connection();
	m_pendingManualConnection.insert(con,url);
	connect(con,&Connection::connected,this,&ClientManager::addManualConnection);
	connect(con,&Connection::hostNotFound,this,&ClientManager::noManualConnection);
	con->connectToHost(url,2424);
}

void ClientManager::addManualConnection()
{
	if(!sender())
		return;

	Connection* con = dynamic_cast<Connection*>(sender());
	m_pendingManualConnection.remove(con);
	addConnection(con);
}

void ClientManager::noManualConnection()
{
	if(!sender())
		return;

	Connection* con = dynamic_cast<Connection*>(sender());
	QString url = m_pendingManualConnection.value(con);
	m_pendingManualConnection.remove(con);

	if(url.isEmpty() || url.isNull())
		return;

	emit manualConnectionFailed(url);
}

void ClientManager::clientAdded(ClientInformation ci)
{
	m_clientsInfo.insert(ci.id(),ci);
	qCDebug(BASE) << this << "Added " << ci.name() << " in ClientManager" ;
	emit userListUpdated();
}

void ClientManager::addMessage(qint16 id, QString msg)
{
	MessageInformation mi;
	mi.setSent(false);
	mi.setUser(m_clientsInfo.value(id).name());
	mi.setMsg(msg);
	m_messages.insert(m_messageCount++,mi);
	emit messageAdded(mi);
}

Client* ClientManager::createClient(IConnection* connection)
{
	Connection *conn = dynamic_cast<Connection*>(connection);
	if(!conn){
		connection->deleteLater();
		return nullptr;
	}

	Client* client = new Client();
	client->setConnection(conn);

	ClientInformation ci;
	ci.setId(m_clientCount++);
	client->setInfo(ci);
	return client;
}

}
