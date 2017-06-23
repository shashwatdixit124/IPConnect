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

#ifndef ICLIENT_H
#define ICLIENT_H

#include <QObject>

namespace IPConnect
{

class IConnection;
class ClientInformation;
/**
 * Allows to access a Client information
 * @author Shashwat Dixit <shashwatdixit124@gmail.com>
 */
class IClient : public QObject
{
	Q_OBJECT

public:
	/**
	 * @brief Returns the connection of the client
	 * @return IConnection
	 */
	virtual IConnection* connection() = 0;

	/**
	 * @brief provides the information of a client
	 * @return ClientInformation
	 */
	virtual ClientInformation info() = 0;

	/**
	 * @brief sends a text message to the client
	 * @param msg message to send to the client
	 */
	virtual void sendMessage(QString msg) = 0;

protected:
	/**
	 * @brief Constructor
	 */
	IClient(QObject* parent = nullptr);

	/**
	 * @brief Destructor
	 */
	~IClient();

};

}

#endif
