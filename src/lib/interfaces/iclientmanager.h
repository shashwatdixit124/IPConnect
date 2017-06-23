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

#ifndef ICLIENTMANAGER_H
#define ICLIENTMANAGER_H

#include <QObject>
#include <QList>

namespace IPConnect
{

class IConnection;
class ClientInformation;
class MessageInformation;

/**
 * Allows access to information of all Registered Clients.
 * Works as an Interface for interacting with a client like sending message.
 * @author Shashwat Dixit <shashwatdixit124@gmail.com>
 */
class IClientManager : public QObject
{
	Q_OBJECT

public:
	/**
	 * @brief initiates a shutdown
	 */
	virtual void shutdown() = 0;

	/**
	 * @brief provides list of information of all the clients
	 * @return List of ClientInformation
	 */
	virtual QList<ClientInformation> clients() = 0;

	/**
	 * @brief provide information for a specific Client
	 * @param id id of a client whose information is needed
	 * @return ClientInformation
	 */
	virtual ClientInformation clientInfo(qint16 id) = 0;

	/**
	 * @brief provides the messages sent-to or recieved-from all the clients
	 * @return List of MessageInformation
	 */
	virtual QList<MessageInformation> messages() = 0;

	/**
	 * @brief sends a message to a Client
	 * @param id id of a client to send message to
	 * @param msg a text message
	 */
	virtual void sendMessage(qint16 id,QString msg) = 0;

	/**
	 * @brief connects to a url specified
	 * @param url an ip address to connect to
	 */
	virtual void connectManualy(QString url) = 0;

	/**
	 * @brief adds a connection to create a Client
	 * @param connection
	 */
	Q_INVOKABLE virtual void addConnection(IConnection* connection) = 0;

Q_SIGNALS:
	/**
	 * @brief will be emitted if new Client is added
	 */
	void userListUpdated();

	/**
	 * @brief will be emitted if new message is sent or recieved
	 * @param mi Information about the message
	 */
	void messageAdded(MessageInformation mi);

	/**
	 * @brief will be emitted if a manual connection is failed
	 * @param url ip address cannot connect to
	 */
	void manualConnectionFailed(QString url);

protected:
	/**
	 * @brief Constructor
	 */
	explicit IClientManager(QObject* parent = nullptr);

	/**
	 * @brief Destructor
	 */
	~IClientManager();

};

}

#endif
