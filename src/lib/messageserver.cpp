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

#include "messageserver.h"

#include "controlcenter.h"
#include "connection.h"
#include "debug.h"
#include "interfaces/iclientmanager.h"
#include "interfaces/iserver.h"

#include <QObject>
#include <QTcpServer>

namespace IPConnect
{

MessageServer::MessageServer(QObject* parent) : IServer(parent){}

MessageServer::~MessageServer(){}

void MessageServer::start()
{
	if(listen(QHostAddress::Any, 2424))
		qCDebug(BASE) << this << "started on 2424";
	else
		qCDebug(BASE) << this << "could not start on 2424";
}

void MessageServer::shutdown()
{
	qCDebug(BASE) << this << "Stopped" ;
	close();
}

void MessageServer::incomingConnection(qintptr handle)
{
	qCDebug(BASE) << this << "new connection " << handle ;
	Connection *conn = new Connection();
	conn->setSocketDescriptor(handle);
	emit gotConnection(conn);
}

}
