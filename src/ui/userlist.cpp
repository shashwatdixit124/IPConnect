#include "userlist.h"

#include <clientinformation.h>

#include <QObject>
#include <QAbstractListModel>
#include <QList>

namespace IPConnect
{

UserList::UserList(QObject* parent) : QAbstractListModel(parent)
{
}

UserList::UserList(QList<IPConnect::ClientInformation> ciList, QObject* parent) 
	: QAbstractListModel(parent)
{
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

void UserList::updateList(QList<IPConnect::ClientInformation>)
{
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
