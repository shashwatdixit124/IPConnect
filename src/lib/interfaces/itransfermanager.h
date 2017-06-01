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

#ifndef ITRANSFERMANAGER_H
#define ITRANSFERMANAGER_H

#include <QObject>
#include <QList>

namespace IPConnect
{

class IConnection;
class Transfer;
class File;

class ITransferManager : public QObject
{
	Q_OBJECT

public:
	virtual void shutdown() = 0;
	virtual void addConnection(IConnection*) = 0;
	virtual QList<Transfer*> pendingTransfers() = 0;
	virtual void sendFile(File) = 0;
	virtual void acceptTransfer(int) = 0;
	virtual void rejectTransfer(int) = 0;

protected:
	explicit ITransferManager(QObject* parent = nullptr);
	~ITransferManager();
};

}

#endif
