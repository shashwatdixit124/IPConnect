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

#include "transfermanager.h"

#include "interfaces/iconnection.h"
#include "interfaces/itransfermanager.h"
#include "interfaces/iusersettings.h"
#include "controlcenter.h"
#include "connection.h"
#include "transfer.h"

#include <QObject>
#include <QList>
#include <QMap>

namespace IPConnect
{

TransferManager::TransferManager(QObject* parent) : ITransferManager(parent) , m_transferCount(0) , m_thread(new QThread())
{
}

TransferManager::~TransferManager()
{
}

void TransferManager::shutdown()
{
	foreach(QThread* t , m_runningThreads.values())
	{
		t->exit();
		t->deleteLater();
	}

	foreach(Transfer* t , m_allTransfers)
	{
		t->moveToThread(thread());
		t->deleteLater();
	}

	m_thread->exit();
	m_thread->deleteLater();
}

void TransferManager::addConnection(IConnection* conn)
{
	Connection* c = dynamic_cast<Connection*>(conn);
	if(!c)
		return;

	Transfer* t = new Transfer();
	connect(t,&Transfer::destroyTransfer,this,&TransferManager::removeTransfer);
	connect(t,&Transfer::requested,this,&TransferManager::addToPending);
	t->setId(m_transferCount++);
	t->setConnection(c);
	m_allTransfers.push_back(t);
	t->moveToThread(m_thread);
}

QList<Transfer*> TransferManager::pendingTransfers()
{
	return m_pendingTransfers.values();
}

void TransferManager::sendFile(File f,QString url)
{
	Connection* c = new Connection();
	m_pendingConnections.insert(c,f);
	connect(c,&Connection::connected,this,&TransferManager::createManualTransfer);
	connect(c,&Connection::errorOccurred,this,&TransferManager::removeManualTransfer);
	c->connectToHost(url,2423);
}

void TransferManager::createManualTransfer()
{
	if(!sender())
		return;

	Connection* c = dynamic_cast<Connection*>(sender());
	if(!c)
		return;

	File f = m_pendingConnections.value(c);
	m_pendingConnections.remove(c);

	Transfer* t = new Transfer();
	t->setConnection(c);
	t->setFile(f);
	t->sendFile();
	t->moveToThread(m_thread);
}

void TransferManager::removeManualTransfer()
{
	if(!sender())
		return;

	Connection* c = dynamic_cast<Connection*>(sender());
	if(!c)
		return;

	c->close();
	m_pendingConnections.remove(c);
}

void TransferManager::removeTransfer()
{
	if(!sender())
		return;

	Transfer* t = dynamic_cast<Transfer*>(sender());
	if(!t)
		return;

	t->moveToThread(thread());
	t->deleteLater();
	QThread* thr = m_runningThreads.value(t->id());
	if(thr) {
		thr->exit();
		thr->deleteLater();
	}
}

void TransferManager::addToPending()
{
	if(!sender())
		return;

	Transfer* t = dynamic_cast<Transfer*>(sender());
	if(!t)
		return;

	m_pendingTransfers.insert(t->id(),t);
	emit pendingTransfersUpdated();
}

}
