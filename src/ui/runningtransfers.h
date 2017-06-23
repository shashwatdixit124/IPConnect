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

#ifndef RUNNINGTRANSFERS_H
#define RUNNINGTRANSFERS_H

#include <QObject>
#include <QAbstractListModel>
#include <QList>

namespace IPConnect
{
class ITransferManager;
class File;

/**
 * @brief List of all Running Transfers for GUI
 * @author Shashwat Dixit <shashwatdixit124@gmail.com>
 */
class RunningTransfers : public QAbstractListModel
{
	Q_OBJECT
public:
	enum UserInfo {
		Id  = Qt::UserRole + 1,
		FileName,
		FilePath,
		FileSize,
		Url,
		ClientName,
		Progress,
		Transfered,
		Rate,
		TimeRemaining
	};

	explicit RunningTransfers(ITransferManager* tm, QObject* parent = nullptr);
	~RunningTransfers();
	int rowCount(const QModelIndex & parent = QModelIndex()) const;
	QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

public Q_SLOTS:
	/**
	 * @brief updates the list
	 */
	void updateList();
	/**
	 * @brief update a specific transfer
	 * @param id of a transfer to update
	 * @param prog percentage of trasnfer completed
	 * @param transfered total transfer completed
	 * @param rate rate of recieving data
	 */
	void updateProgress(qint16,int,quint64,int);

protected:
	QHash<int, QByteArray> roleNames() const;
	ITransferManager *m_tm;
	QList<File> m_transfers;

	/**
	 * @brief converts a no in bytes to a string in B/KB/MB/GB
	 * @param bytes to convert
	 * @return converted string
	 */
	QString bytesToString(quint64 bytes) const;

	/**
	 * @brief converts time remaining for a file from integer in sec to min in string
	 * @param f file to convert time for
	 * @return converted string
	 */
	QString timeForFile(File f) const;

};

}

#endif

