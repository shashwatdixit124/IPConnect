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

#ifndef CLIENT_H
#define CLIENT_H

#include "interfaces/iclient.h"
#include "clientinformation.h"
#include "message.h"

#include <QObject>
#include <QMap>

namespace IPConnect
{

class IConnection;
class SecureTunnel;

/**
 * @brief Represents a user on other side of connection
 *
 * @see IClient
 */
class Client : public IClient
{
	Q_OBJECT

public:
	/**
	 * @brief CTOR
	 */
	explicit Client(QObject* parent = nullptr);

	/**
	 * @brief DTOR
	 */
	~Client();

	/**
	 * @brief connection of the client
	 * @return IConnection
	 */
	IConnection* connection() override;

	/**
	 * @brief information of Client
	 * @return ClientInformation
	 */
	ClientInformation info() override;

	/**
	 * @brief sends messages to the client
	 * @param msg text message
	 */
	void sendMessage(QString msg) override;

	/**
	 * @brief sets the connection for client
	 * @param conn Connection for client
	 */
	void setConnection(IConnection* conn);

	/**
	 * @brief set the information for client
	 * @param info ClientInformation for Client
	 */
	void setInfo(ClientInformation info);

	/**
	 * @brief starts the client
	 * creates a secure connection for sending and recieveing messages
	 */
	void start();

	/**
	 * @brief to check if client has accepted information or not
	 * @return true if accepted data, false otherwise
	 */
	bool hasAcceptedData() const;

public Q_SLOTS:
	/**
	 * @brief handle the data available on Connection
	 */
	void handleRead();

	/**
	 * @brief close the connection of Client
	 */
	void closeConnection();

Q_SIGNALS:
	/**
	 * @brief emitted when data has been accepted
	 * @param ci ClientInformation recieved (or updated)
	 */
	void infoRecieved(ClientInformation ci);

	/**
	 * @brief emitted when a message is recieved
	 * @param id of client
	 * @param msg text message
	 */
	void messageRecieved(qint16 id, QString msg);

	/**
	 * @brief emitted when connection is closed
	 */
	void connectionClosed();

protected:
	/**
	 * @brief handle the request
	 * i.e. Process the Message recieved and take appropriate action according to Method and Option of Request
	 */
	void handleRequest();

	/**
	 * @brief sends the Message
	 * @param m Message to send
	 */
	void send(Message m);

	/**
	 * @brief sends our detail to the client
	 * i.e. ip address , name
	 */
	void sendDetail();

	/**
	 * @brief action to take when connection is secured
	 */
	void secured();

	/**
	 * @brief Connection to read from and write to
	 */
	IConnection* m_conn;

	/**
	 * @brief tunnel that makes reading and sending secure
	 */
	SecureTunnel* m_tunnel;

	/**
	 * @brief information about client
	 */
	ClientInformation m_info;

	/**
	 * @brief request recieved
	 */
	Message m_request;

	/**
	 * @brief if detail accepted or not
	 */
	bool m_detailAccepted;

	/**
	 * @brief if detail sent or not
	 */
	bool m_detailSent;
};

}

#endif
