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

#include "messagelist.h"

#include <interfaces/iclientmanager.h>
#include <messageinformation.h>

#include <QObject>
#include <QAbstractListModel>
#include <QList>

namespace IPConnect
{

MessageList::MessageList(IClientManager* cm, QObject* parent) : QAbstractListModel(parent) , m_cm(cm)
{
	connect(cm,&IClientManager::messageAdded,this,&MessageList::updateList);
	QList<MessageInformation> miList = cm->messages();
	beginInsertRows(QModelIndex(), 0, miList.count()-1);
	m_messages = miList;
	endInsertRows();
}

MessageList::~MessageList()
{
}

int MessageList::rowCount(const QModelIndex& parent) const
{
	Q_UNUSED(parent);
	return m_messages.count();
}

QVariant MessageList::data(const QModelIndex& index, int role) const
{
	if (index.row() < 0 || index.row() >= m_messages.count())
		return QVariant();

	const MessageInformation &msg = m_messages[index.row()];
	if (role == Sent)
		return msg.sent();
	else if (role == User)
		return msg.user();
	else if (role == Message)
		return msg.msg();
	return QVariant();
}

void MessageList::updateList(MessageInformation mi)
{
	beginInsertRows(QModelIndex(), rowCount(), rowCount());
	m_messages.append(mi);
	endInsertRows();
	emit messagesAdded(1);
}

QHash<int, QByteArray> MessageList::roleNames() const
{
	QHash<int, QByteArray> roles;
	roles[Sent] = "sent";
	roles[User] = "user";
	roles[Message] = "message";
	return roles;
}

}

