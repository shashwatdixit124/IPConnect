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

#ifndef TRANSFERSERVER_H
#define TRANSFERSERVER_H

#include "interfaces/iserver.h"

#include <QObject>

namespace IPConnect
{

/**
 * @brief Server that handles all connection related to Transfers.
 * server listens on port 2423
 *
 * @see IServer
 * @author Shashwat Dixit <shashwatdixit124@gmail.com>
 */
class TransferServer : public IServer
{
	Q_OBJECT

public:
	/**
	 * @brief CTOR
	 */
	explicit TransferServer(QObject* parent);

	/**
	 * @brief DTOR
	 */
	~TransferServer();

	/**
	 * @brief start the server
	 */
	void start() override;

	/**
	 * @brief closes the server
	 */
	void shutdown();

protected:
	QTcpServer m_server;

	/**
	 * @brief creates a new IConnection when connection availale
	 * @param handle socket descriptor
	 */
	void incomingConnection(qintptr handle) override;

};

}

#endif
 

