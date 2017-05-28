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

#ifndef CLIENTMANAGER_H
#define CLIENTMANAGER_H

#include "interfaces/iclientmanager.h"
#include "messageinformation.h"

#include <QObject>
#include <QList>
#include <QMap>
#include <QThread>

namespace IPConnect
{

class IConnection;
class Client;
class ClientInformation;
class ClientThreadManager;

class ClientManager : public IClientManager
{
	Q_OBJECT

public:
	explicit ClientManager(QObject* parent = nullptr);
	~ClientManager();

	void shutdown() override;
	QList<ClientInformation> clients() override;
	QList<MessageInformation> messages() override;
	void addConnection(IConnection*) override;
	void sendMessage(qint16,QString) override;
	void connectManualy(QString) override;

Q_SIGNALS:
	void clientCreated(Client*);
	void removeAllClients();
	void sendToClient(qint16,QString);

public Q_SLOTS:
	void clientAdded(ClientInformation);
	void addMessage(qint16,QString);
	void clientRemoved(qint16);

protected:
	Client* createClient(IConnection*);
	void addManualConnection();

	QMap<qint16,ClientInformation> m_clientsInfo;
	QMap<qint16,MessageInformation> m_messages;
	qint16 m_clientCount;
	qint16 m_messageCount;
	QThread *m_clientThread;
	ClientThreadManager *m_clientThreadManager;

};

}

#endif
