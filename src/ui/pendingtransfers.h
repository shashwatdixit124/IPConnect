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

#ifndef PENDINGTRANSFERS_H
#define PENDINGTRANSFERS_H

#include <QObject>
#include <QAbstractListModel>
#include <QList>

namespace IPConnect
{
class ITransferManager;
class File;

class PendingTransfers : public QAbstractListModel
{
	Q_OBJECT
public:
	enum UserInfo {
		Id  = Qt::UserRole + 1,
		FileName,
		FilePath,
		FileSize,
		Url,
		ClientName
	};

	explicit PendingTransfers(ITransferManager* tm, QObject* parent = nullptr);
	~PendingTransfers();
	int rowCount(const QModelIndex & parent = QModelIndex()) const;
	QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

public Q_SLOTS:
	void updateList();

protected:
	QHash<int, QByteArray> roleNames() const;
	ITransferManager *m_tm;
	QList<File> m_transfers;

	QString bytesToString(qint32) const;

};

}

#endif

