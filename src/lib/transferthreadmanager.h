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

class TransferThreadManager : public QObject
{
	Q_OBJECT

public:
	explicit TransferThreadManager(QObject* parent = nullptr);
	~TransferThreadManager();

public Q_SLOTS:
	void transferCreated(Transfer*);
	void manualTransferCreated(Transfer*);
	void acceptPending(qint16);
	void rejectPending(qint16);
	void stopTransfer(qint16);
	void progressTransfer(int,quint64,int);

	void requested();
	void destroyTransfer();

	void setThread(QThread*);

Q_SIGNALS:
	void requestedTransfer(File);
	void transferProgressed(qint16,int,quint64,int);
	void transferRemoved(qint16);
	void accepted(qint16);
	void rejected(qint16);

protected:
	QMap<qint16,Transfer*> m_transferList;

	void removeTransfer(Transfer*);

};

}

#endif
