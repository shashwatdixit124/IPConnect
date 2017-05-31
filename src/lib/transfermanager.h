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

#ifndef TRANSFERMANAGER_H
#define TRANSFERMANAGER_H

#include "interfaces/itransfermanager.h"

#include <QObject>
#include <QList>
#include <QMap>
#include <QThread>

namespace IPConnect
{

class IConnection;
class Transfer;

class TransferManager : public ITransferManager
{
	Q_OBJECT

public:
	explicit TransferManager(QObject* parent = nullptr);
	~TransferManager();

	void shutdown() override;
	void addConnection(IConnection*) override;
	QList<Transfer*> pendingTransfers() override;

public Q_SLOTS:
	void removeTransfer();
	void addToPending();

Q_SIGNALS:
	void pendingTransfersUpdated();

protected:
	QMap<int,Transfer*> m_pendingTransfers;
	QMap<int,QThread*> m_runningThreads;
	QList<Transfer*> m_allTransfers;
	QThread* m_thread;
	int m_transferCount;

};

}

#endif
