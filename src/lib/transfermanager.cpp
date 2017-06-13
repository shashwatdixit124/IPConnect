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
#include "debugtransfer.h"
#include "transfer.h"
#include "transferthreadmanager.h"

#include <QObject>
#include <QList>
#include <QMap>

namespace IPConnect
{

TransferManager::TransferManager(QObject* parent) : ITransferManager(parent) ,
	m_transferCount(0) , m_transferThread(new QThread(this)) , m_ttm(new TransferThreadManager())
{
	qCDebug(TRANSFER) << this << "Started";
	qRegisterMetaType<QThread*>("QThread*");
	connect(m_ttm,&TransferThreadManager::requestedTransfer,this,&TransferManager::addToPending,Qt::QueuedConnection);
	connect(m_ttm,&TransferThreadManager::accepted,this,&TransferManager::accepted,Qt::QueuedConnection);
	connect(m_ttm,&TransferThreadManager::rejected,this,&TransferManager::rejected,Qt::QueuedConnection);
	connect(m_ttm,&TransferThreadManager::transferRemoved,this,&TransferManager::transferRemoved,Qt::QueuedConnection);
	connect(this,&TransferManager::transferCreated,m_ttm,&TransferThreadManager::transferCreated,Qt::QueuedConnection);
	connect(this,&TransferManager::acceptPending,m_ttm,&TransferThreadManager::acceptPending,Qt::QueuedConnection);
	connect(this,&TransferManager::rejectPending,m_ttm,&TransferThreadManager::rejectPending,Qt::QueuedConnection);
	connect(this,&TransferManager::removeTransfer,m_ttm,&TransferThreadManager::stopTransfer,Qt::QueuedConnection);
	connect(this,&TransferManager::manualTransferCreated,m_ttm,&TransferThreadManager::manualTransferCreated,Qt::QueuedConnection);
	m_transferThread->start();
	m_ttm->moveToThread(m_transferThread);
}

TransferManager::~TransferManager()
{
}

void TransferManager::shutdown()
{
	QMetaObject::invokeMethod(m_ttm,"setThread",Qt::QueuedConnection,Q_ARG(QThread*,thread()));
	m_ttm->deleteLater();
	m_transferThread->quit();
	m_transferThread->wait();
	m_transferThread->deleteLater();
	qCDebug(TRANSFER) << this << "Stopped";
}

void TransferManager::addConnection(IConnection* connection)
{
	qCDebug(TRANSFER) << this << "adding new Transfer with descriptor " <<  connection->socketDescriptor();
	Transfer* transfer = createTransfer(connection);

	if(!transfer){
		qCDebug(TRANSFER) << "cannot add Client with socket descriptor " << connection->socketDescriptor() ;
		return;
	}

	//transfer->setThread(m_transferThread);
	transfer->connection()->moveToThread(m_transferThread);
	transfer->moveToThread(m_transferThread);
	emit transferCreated(transfer);
}

QList<File> TransferManager::pendingTransfers()
{
	return m_pendingTransfers.values();
}

QList<File> TransferManager::runningTransfers()
{
	return m_runningTransfers.values();
}

void TransferManager::sendFile(File f)
{
	Connection* c = new Connection();
	m_pendingConnections.insert(c,f);
	connect(c,&Connection::connected,this,&TransferManager::createManualTransfer);
	connect(c,&Connection::errorOccurred,this,&TransferManager::removeManualTransfer);
	c->connectToHost(f.url(),2423);
}

void TransferManager::createManualTransfer()
{
	if(!sender())
		return;

	Connection* c = dynamic_cast<Connection*>(sender());

	if(!c)
		return;

	disconnect(c,&Connection::connected,this,&TransferManager::createManualTransfer);
	disconnect(c,&Connection::errorOccurred,this,&TransferManager::removeManualTransfer);

	File f = m_pendingConnections.value(c);
	m_pendingConnections.remove(c);
	f.setId(m_transferCount++);

	Transfer* t = new Transfer();
	t->setConnection(c);
	t->setFile(f);
	m_allTransfers.insert(f.id(),f);
	//t->setThread(m_transferThread);
	t->connection()->moveToThread(m_transferThread);
	t->moveToThread(m_transferThread);
	emit manualTransferCreated(t);
}

void TransferManager::removeManualTransfer()
{
	if(!sender())
		return;

	Connection* c = dynamic_cast<Connection*>(sender());
	if(!c)
		return;

	c->close();
	c->deleteLater();
	m_pendingConnections.remove(c);
}

void TransferManager::addToPending(File f)
{
	qCDebug(TRANSFER) << this << "adding to pending " << f.name() ;

	if(!sender())
		return;

	qint16 id = f.id();
	m_allTransfers.remove(id);
	m_allTransfers.insert(id,f);
	m_pendingTransfers.insert(id,f);
	emit pendingTransfersUpdated();
}

void TransferManager::transferRemoved(qint16 id)
{
	if(m_pendingTransfers.contains(id)) {
		m_pendingTransfers.remove(id);
		emit pendingTransfersUpdated();
	}
	if(m_runningTransfers.contains(id)) {
		m_runningTransfers.remove(id);
		emit runningTransfersUpdated();
	}
}

void TransferManager::accepted(qint16 id)
{
	File f = m_pendingTransfers.value(id);
	m_pendingTransfers.remove(id);
	m_runningTransfers.insert(id,f);
	emit pendingTransfersUpdated();
	emit runningTransfersUpdated();
}

void TransferManager::rejected(qint16 id)
{
	File f = m_pendingTransfers.value(id);
	m_pendingTransfers.remove(id);
	emit pendingTransfersUpdated();
}

void TransferManager::acceptTransfer(qint16 id)
{
	emit acceptPending(id);
}

void TransferManager::rejectTransfer(qint16 id)
{
	emit rejectPending(id);
}

void TransferManager::stopTransfer(qint16 id)
{
	emit removeTransfer(id);
}

Transfer* TransferManager::createTransfer(IConnection* conn)
{
	if(!conn)
		return nullptr;

	Transfer* transfer = new Transfer();
	transfer->setConnection(conn);

	File f;
	f.setId(m_transferCount++);
	transfer->setFile(f);
	return transfer;
}

}
