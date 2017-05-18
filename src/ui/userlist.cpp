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
