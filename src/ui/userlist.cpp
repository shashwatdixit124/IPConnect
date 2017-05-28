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

#include "userlist.h"

#include <interfaces/iclientmanager.h>
#include <clientinformation.h>

#include <QObject>
#include <QAbstractListModel>
#include <QList>

namespace IPConnect
{

IPConnect::UserList::UserList(IClientManager* cm, QObject* parent) : QAbstractListModel(parent) , m_cm(cm)
{
	connect(cm,&IClientManager::userListUpdated,this,&UserList::updateList);
	QList<ClientInformation> ciList = cm->clients();
	beginInsertRows(QModelIndex(), 0, ciList.count()-1);
	m_users = ciList;
	endInsertRows();
}

UserList::~UserList()
{
}

int UserList::rowCount(const QModelIndex& parent) const
{
	Q_UNUSED(parent);
	return m_users.count();
}

QVariant UserList::data(const QModelIndex& index, int role) const
{
	if (index.row() < 0 || index.row() >= m_users.count())
		return QVariant();

	const ClientInformation &user = m_users[index.row()];
	if (role == Id)
		return user.id();
	else if (role == Name)
		return user.name();
	else if (role == Ip)
		return user.ip();
	return QVariant();
}

void UserList::updateList()
{
	QList<ClientInformation> ciList = m_cm->clients();
	beginRemoveRows(QModelIndex(),0,rowCount()-1);
	m_users.clear();
	endRemoveRows();
	beginInsertRows(QModelIndex(), 0, ciList.count()-1);
	m_users = ciList;
	endInsertRows();
}

QHash<int, QByteArray> UserList::roleNames() const
{
	QHash<int, QByteArray> roles;
	roles[Id] = "id";
	roles[Name] = "name";
	roles[Ip] = "ip";
	return roles;
}

}
