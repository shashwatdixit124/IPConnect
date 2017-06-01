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

#include "pendingtransfers.h"

#include <interfaces/itransfermanager.h>
#include <transfer.h>
#include <file.h>

#include <QObject>
#include <QAbstractListModel>
#include <QList>

namespace IPConnect
{

PendingTransfers::PendingTransfers(ITransferManager* tm, QObject* parent) : QAbstractListModel(parent) , m_tm(tm)
{
	connect(tm,&ITransferManager::pendingTransfersUpdated,this,&PendingTransfers::updateList);
	m_transfers = tm->pendingTransfers();
	beginInsertRows(QModelIndex(), 0, m_transfers.count()-1);
	endInsertRows();
}

PendingTransfers::~PendingTransfers()
{
}

int PendingTransfers::rowCount(const QModelIndex& parent) const
{
	Q_UNUSED(parent);
	return m_transfers.count();
}

QVariant PendingTransfers::data(const QModelIndex& index, int role) const
{
	if (index.row() < 0 || index.row() >= m_transfers.count())
		return QVariant();

	const File &file = m_transfers[index.row()]->file();
	if (role == FileName)
		return file.name();
	else if (role == FilePath)
		return file.path();
	else if (role == FileSize)
		return file.size();
	else if (role == Url)
		return file.url();
	else if (role == ClientName)
		return file.userName();
	return QVariant();
}

void PendingTransfers::updateList()
{
	m_transfers = m_tm->pendingTransfers();
	beginRemoveRows(QModelIndex(),0,rowCount()-1);
	endRemoveRows();
	beginInsertRows(QModelIndex(), 0 , m_transfers.count()-1);
	endInsertRows();
}

QHash<int, QByteArray> PendingTransfers::roleNames() const
{
	QHash<int, QByteArray> roles;
	roles[FileName] = "filename";
	roles[FilePath] = "filepath";
	roles[FileSize] = "filesize";
	roles[Url] = "url";
	roles[ClientName] = "clientname";
	return roles;
}

}


