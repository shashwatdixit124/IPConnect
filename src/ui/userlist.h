#ifndef USERLIST_H
#define USERLIST_H

#include <QObject>
#include <QAbstractListModel>
#include <QList>

namespace IPConnect
{
class ClientInformation;

class UserList : public QAbstractListModel
{
	Q_OBJECT
public:
	enum UserInfo {
		Name = Qt::UserRole + 1,
		Id,
		Ip
	};

	explicit UserList(QObject* parent = nullptr);
	UserList(QList<ClientInformation>,QObject* parent = nullptr);
	~UserList();
	int rowCount(const QModelIndex & parent = QModelIndex()) const;
	QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

public Q_SLOTS:
	void updateList(QList<ClientInformation>);

protected:
	QHash<int, QByteArray> roleNames() const;

private:
	QList<ClientInformation> m_users;

};

}

#endif
