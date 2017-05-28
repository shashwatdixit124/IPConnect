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

#include "server.h"

#include "controlcenter.h"
#include "connection.h"
#include "debug.h"
#include "interfaces/iclientmanager.h"

#include <QObject>
#include <QTcpServer>

namespace IPConnect
{

Server::Server(QObject* parent) : IServer(parent){}

Server::~Server(){}

void Server::start()
{
	if(listen(QHostAddress::Any, 2424))
		qCDebug(BASE) << "Server started on 2424";
	else
		qCDebug(BASE) << "Server could not start on 2424";
}

void Server::shutdown()
{
	qCDebug(BASE) << "Server Stopped" ;
	close();
}

void Server::incomingConnection(qintptr handle)
{
	qCDebug(BASE) << "handling connection with descriptor " << handle ;
	Connection *conn = new Connection();
	conn->setSocketDescriptor(handle);
	if(conn){
		ControlCenter::instance()->clientManager()->addConnection(conn);
	}
}

}
