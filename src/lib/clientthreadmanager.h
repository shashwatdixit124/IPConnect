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

#ifndef CLIENTTHREADMANAGER_H
#define CLIENTTHREADMANAGER_H

#include <QObject>
#include <QMap>
#include <QVector>

namespace IPConnect
{
class Client;
class ClientInformation;

class ClientThreadManager : public QObject
{
	Q_OBJECT
public:
	explicit ClientThreadManager(QObject* parent = nullptr);
	~ClientThreadManager();

Q_SIGNALS:
	void clientAdded(ClientInformation);
	void messageAdded(qint16,QString);
	void clientRemoved(qint16);

public Q_SLOTS:
	void clientCreated(Client*);
	void addClient(ClientInformation);
	void messageRecieved(qint16,QString);
	void closeConnection(Client*);
	void removeAllClients();
	void removeClient();
	void sendToClient(qint16,QString);

protected:
	QMap<qint16,Client*> m_clientList;
	QVector<Client*> m_clientInQueue;

	void addToPending(Client*);
	bool isPending(Client*);
	void removeIfPending(Client*);

};

}

#endif
