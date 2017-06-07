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

#include "transferserver.h"

#include "controlcenter.h"
#include "connection.h"
#include "debugtransfer.h"
#include "interfaces/itransfermanager.h"

#include <QObject>
#include <QTcpServer>

namespace IPConnect
{

TransferServer::TransferServer(QObject* parent) : IServer(parent){}

TransferServer::~TransferServer(){}

void TransferServer::start()
{
	if(listen(QHostAddress::Any, 2423))
		qCDebug(TRANSFER) << this << "started on 2423";
	else
		qCDebug(TRANSFER) << this << "could not start on 2423";
}

void TransferServer::shutdown()
{
	qCDebug(TRANSFER) << this << "Stopped" ;
	close();
}

void TransferServer::incomingConnection(qintptr handle)
{
	qCDebug(TRANSFER) << this << "new connection " << handle ;
	Connection *conn = new Connection();
	conn->setSocketDescriptor(handle);
	emit gotConnection(conn);
}

}

