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

#include "runningtransfers.h"

#include <interfaces/itransfermanager.h>
#include <transfer.h>
#include <file.h>

#include <QObject>
#include <QAbstractListModel>
#include <QList>
#include <QDebug>

namespace IPConnect
{

RunningTransfers::RunningTransfers(ITransferManager* tm, QObject* parent) : QAbstractListModel(parent) , m_tm(tm)
{
	connect(tm,&ITransferManager::runningTransfersUpdated,this,&RunningTransfers::updateList);
	connect(tm,&ITransferManager::transferProgressed,this,&RunningTransfers::updateProgress);
	m_transfers = tm->runningTransfers();
	beginInsertRows(QModelIndex(), 0, m_transfers.count()-1);
	endInsertRows();
}

RunningTransfers::~RunningTransfers()
{
}

int RunningTransfers::rowCount(const QModelIndex& parent) const
{
	Q_UNUSED(parent);
	return m_transfers.count();
}

QVariant RunningTransfers::data(const QModelIndex& index, int role) const
{
	if (index.row() < 0 || index.row() >= m_transfers.count())
		return QVariant();

	const File &file = m_transfers[index.row()];
	if (role == Id)
		return file.id();
	else if (role == FileName)
		return file.name();
	else if (role == FilePath)
		return file.path();
	else if (role == FileSize)
		return bytesToString(file.size());
	else if (role == Url)
		return file.url();
	else if (role == ClientName)
		return file.userName();
	else if (role == Progress)
		return file.progress();
	else if(role == Transfered)
		return bytesToString(file.transfered());
	else if(role == Rate)
		return file.rate();
	else if(role == TimeRemaining)
		return timeForFile(file);
	return QVariant();
}

void RunningTransfers::updateList()
{
	if(rowCount() > 0) {
		beginRemoveRows(QModelIndex(),0,rowCount()-1);
		endRemoveRows();
	}

	m_transfers = m_tm->runningTransfers();

	if(m_transfers.count() <= 0)
		return;

	beginInsertRows(QModelIndex(), 0 , m_transfers.count()-1);
	endInsertRows();
}

void RunningTransfers::updateProgress(qint16 id, int prog, quint64 transfered, int rate)
{
	qint16 i ;
	File f;
	for(i=0;i<rowCount();i++)
	{
		if(m_transfers.value(i).id() == id){
			f = m_transfers.value(i);
			break;
		}
	}

	beginRemoveRows(QModelIndex(),i, i);
	endRemoveRows();

	beginInsertRows(QModelIndex(), i, i);
	m_transfers[i].setProgress(prog);
	m_transfers[i].setTransfered(transfered);
	m_transfers[i].setRate(rate);
	endInsertRows();

}

QHash<int, QByteArray> RunningTransfers::roleNames() const
{
	QHash<int, QByteArray> roles;
	roles[Id] = "id";
	roles[FileName] = "filename";
	roles[FilePath] = "filepath";
	roles[FileSize] = "filesize";
	roles[Url] = "url";
	roles[ClientName] = "clientname";
	roles[Progress] = "progress";
	roles[Transfered] = "transfered";
	roles[Rate] = "rate";
	roles[TimeRemaining] = "timeRemaining";
	return roles;
}

QString RunningTransfers::bytesToString(quint64 bytes) const
{
	if(bytes > 1024*1024*1024)
		return QString::number((double)bytes/(1024*1024*1024)) + QString(" GB");
	else if(bytes > 1024*1024)
		return QString::number(bytes/(1024*1024)) + QString(" MB");
	else if(bytes > 1024)
		return QString::number(bytes/1024) + QString(" KB");
	else
		return QString::number(bytes) + QString(" B");
}

QString RunningTransfers::timeForFile(File f) const
{
	if(f.rate() == 0)
		return QString("Unknown Time");

	quint64 secs = (f.size() - f.transfered())/(f.rate()*1024*1024) ;
	if(secs < 60)
		return QString::number(secs) + " sec";
	else if(secs < 60*60)
	{
		int min = secs / 60;
		int sec = secs % 60;
		return QString::number(min)+" min "+QString::number(sec)+" sec";
	}
	else
	{
		int hour = secs / (60*60) ;
		int min = secs / 60 ;
		return QString::number(hour)+" h "+QString::number(min)+" min";
	}
}

}


