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

#ifndef TRANSFERTHREADMANAGER_H
#define TRANSFERTHREADMANAGER_H

#include <QObject>
#include <QList>
#include <QThread>
#include <QMap>

namespace IPConnect
{

class File;
class Transfer;

/**
 * @brief Manages every Transfer instance on different thread
 * @author Shashwat Dixit <shashwatdixit124@gmail.com>
 */
class TransferThreadManager : public QObject
{
	Q_OBJECT

public:
	/**
	 * @brief CTOR
	 */
	explicit TransferThreadManager(QObject* parent = nullptr);

	/**
	 * @brief DTOR
	 */
	~TransferThreadManager();

public Q_SLOTS:
	/**
	 * @brief adds the created Transfer to list of transfers
	 * @param transfer Transfer to add to list
	 */
	void transferCreated(Transfer* transfer);

	/**
	 * @brief adds a Manually Created Trasnfer to list of transfers
	 * @param transfer Trasnfer to add to list
	 */
	void manualTransferCreated(Transfer* transfer);

	/**
	 * @brief accepts a pending transfer specified by the id provided
	 * @param id of transfer to accept
	 */
	void acceptPending(qint16 id);

	/**
	 * @brief rejects a pending transfer specified by the id provided
	 * @param id of transfer to reject
	 */
	void rejectPending(qint16 id);

	/**
	 * @brief stops a transfer
	 * @param id of transfer to stop
	 */
	void stopTransfer(qint16 id);

	/**
	 * @brief emits transferProgressed ;
	 * @param prog percentage complete of a transfer
	 * @param transfered total data transfered
	 * @param rate rate at which recieving data
	 */
	void progressTransfer(int prog, quint64 transfered, int rate);

	/**
	 * @brief emits trasnferRequested
	 */
	void requested();

	/**
	 * @brief removes transfer
	 */
	void destroyTransfer();

	/**
	 * @brief move to thread
	 * @param thr trread to move to
	 */
	void setThread(QThread* thr);

Q_SIGNALS:
	/**
	 * @brief emitted when a new Trasnfer is requested
	 * @param f File information of requested transfer
	 */
	void requestedTransfer(File f);

	/**
	 * @brief emitted when any running transfer is progressed
	 * @param id id of transfer progressed
	 * @param prog percentage complete of transfer
	 * @param transfered total data transfered
	 * @param rate rate at which recieving data
	 */
	void transferProgressed(qint16 id, int prog, quint64 transfered, int rate);

	/**
	 * @brief emitted when a transfer is removed
	 * @param id of a trasnfer removed
	 */
	void transferRemoved(qint16 id);

	/**
	 * @brief emitted if a trasnfer is accepted
	 * @param id of a trasnfer accepted
	 */
	void accepted(qint16 id);

	/**
	 * @brief emitted if a trasnfer is rejected
	 * @param id of a trasnfer rejected
	 */
	void rejected(qint16 id);

protected:
	/**
	 * @brief Map of all Trasnfers
	 */
	QMap<qint16,Transfer*> m_transferList;

	/**
	 * @brief removes transfer
	 * @param t Trasnfer to remove
	 */
	void removeTransfer(Transfer* t);

};

}

#endif
