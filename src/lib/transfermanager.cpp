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
#include "connection.h"
#include "transfer.h"

#include <QObject>

namespace IPConnect
{

TransferManager::TransferManager(QObject* parent) : ITransferManager(parent)
{
}

TransferManager::~TransferManager()
{
}

void TransferManager::shutdown()
{
	foreach(Transfer* t , m_runningTransfers.keys())
	{
		t->moveToThread(thread());
		t->stop();
		t->deleteLater();
	}

	foreach(QThread* t , m_runningTransfers.values())
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
	t->setConnection(c);
	m_allTransfers.push_back(t);
	t->moveToThread(m_thread);
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
	QThread* thr = m_runningTransfers.value(t);
	thr->exit();
	thr->deleteLater();
}

void TransferManager::addToPending()
{
	if(!sender())
		return;

	Transfer* t = dynamic_cast<Transfer*>(sender());
	if(!t)
		return;

	m_pendingTransfers.insert(t,0);
	emit pendingTransfersUpdated();
}

}
