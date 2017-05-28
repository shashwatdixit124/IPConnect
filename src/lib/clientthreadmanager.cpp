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

#include "clientthreadmanager.h"

#include "client.h"
#include "connection.h"
#include "debug.h"
#include "clientinformation.h"

#include <QThread>
#include <QObject>
#include <QMap>

namespace IPConnect
{

ClientThreadManager::ClientThreadManager(QObject* parent) : QObject(parent)
{
}

ClientThreadManager::~ClientThreadManager()
{
}

void ClientThreadManager::clientCreated(Client* client)
{
	qint16 id = client->info().id();
	m_clientList.insert(id,client);
	connect(client,&Client::infoRecieved,this,&ClientThreadManager::addClient);
	connect(client,&Client::messageRecieved,this,&ClientThreadManager::messageRecieved);
	connect(client,&Client::connectionClosed,this,&ClientThreadManager::removeClient);
	qCDebug(BASE) << this << "Client Added with id " << id ;
	client->start();
}

void ClientThreadManager::addClient(ClientInformation ci)
{
	qint16 id = ci.id();
	Client* client = m_clientList.value(id);
	client->setInfo(ci);
	emit clientAdded(ci);
}

void ClientThreadManager::messageRecieved(qint16 id, QString msg)
{
	emit messageAdded(id,msg);
}

void ClientThreadManager::removeAllClients()
{
	QList<Client*> clients = m_clientList.values();
	foreach(Client* c , clients){
		closeConnection(c);
		c->deleteLater();
	}
}

void ClientThreadManager::closeConnection(Client* client)
{
	IConnection* conn = client->connection();
	if(conn){
		if(conn->isValid())
			conn->close();
		conn->deleteLater();
	}
}

void ClientThreadManager::removeClient()
{
	if(!sender())
		return;

	Client* client = dynamic_cast<Client*>(sender());
	qint16 id = client->info().id();
	client->deleteLater();
	m_clientList.remove(id);
	emit clientRemoved(id);
}

void ClientThreadManager::sendToClient(qint16 id, QString msg)
{
	Client* client = m_clientList.value(id);
	if(!client)
	{
		qCDebug(BASE) << this << "sending message " << msg << " to client " << id  ;
		return;
	}
	client->sendMessage(msg);
}

}
