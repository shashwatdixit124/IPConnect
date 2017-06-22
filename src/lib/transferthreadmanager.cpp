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

#include "transferthreadmanager.h"

#include "transfer.h"
#include "debugtransfer.h"

#include <QObject>
#include <QThread>
#include <QDebug>

namespace IPConnect
{

TransferThreadManager::TransferThreadManager(QObject* parent) : QObject(parent)
{
	qRegisterMetaType<QThread*>("QThread*");
}

TransferThreadManager::~TransferThreadManager()
{
	foreach(Transfer* t , m_transferList.values())
		removeTransfer(t);
}

void TransferThreadManager::transferCreated(Transfer* transfer)
{
	if(!transfer)
		return;

	connect(transfer,&Transfer::requested,this,&TransferThreadManager::requested,Qt::QueuedConnection);
	connect(transfer,&Transfer::progress,this,&TransferThreadManager::progressTransfer,Qt::QueuedConnection);
	connect(transfer,&Transfer::destroyTransfer,this,&TransferThreadManager::destroyTransfer,Qt::QueuedConnection);
	connect(transfer,&Transfer::error,this,&TransferThreadManager::destroyTransfer,Qt::QueuedConnection);
	qint16 id = transfer->file().id();
	m_transferList.insert(id,transfer);
	qCDebug(TRANSFER) << this << "Transfer Added with id " << id ;
}

void TransferThreadManager::manualTransferCreated(Transfer* transfer)
{
	if(!transfer)
		return;

	qint16 id = transfer->file().id();
	m_transferList.insert(id,transfer);
	transfer->createTunnel();
	qCDebug(TRANSFER) << this << "Manual Transfer Added with id " << id;
}

void TransferThreadManager::acceptPending(qint16 id)
{
	Transfer* t = m_transferList.value(id);
	if(!t)
		return;
	t->accept();
	emit accepted(id);
}

void TransferThreadManager::rejectPending(qint16 id)
{
	Transfer* t = m_transferList.value(id);
	if(!t)
		return;
	t->reject();
	emit rejected(id);
}

void TransferThreadManager::stopTransfer(qint16 id)
{
	Transfer* t = m_transferList.value(id);
	if(!t)
		return;
	QMetaObject::invokeMethod(t,"stop",Qt::QueuedConnection);
}

void TransferThreadManager::progressTransfer(int prog, quint64 transfered , int rate)
{
	if(!sender())
		return;

	Transfer* t = dynamic_cast<Transfer*>(sender());
	emit transferProgressed(t->file().id(),prog,transfered,rate);
}

void TransferThreadManager::requested()
{
	if(!sender())
		return;

	Transfer* t = dynamic_cast<Transfer*>(sender());
	File f = t->file();
	qint16 id = f.id();
	//m_transferList.value(id)->setFile(f);
	emit requestedTransfer(f);
}

void TransferThreadManager::destroyTransfer()
{
	if(!sender())
		return;

	Transfer* t = dynamic_cast<Transfer*>(sender());
	removeTransfer(t);
}

void TransferThreadManager::setThread(QThread* thr)
{
	moveToThread(thr);
}

void TransferThreadManager::removeTransfer(Transfer* t)
{
	QMetaObject::invokeMethod(t,"setThread",Qt::QueuedConnection,Q_ARG(QThread*,thread()));
	qint16 id = t->file().id();
	t->deleteLater();
	m_transferList.remove(id);
	emit transferRemoved(id);
}

}
