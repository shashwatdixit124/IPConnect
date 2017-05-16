#ifndef UIMANAGER_H
#define UIMANAGER_H

#include <clientinformation.h>

#include <QObject>
#include <QAbstractListModel>

namespace IPConnect
{
class IControlCenter;
class UserList;

class UiManager : public QObject
{
	Q_OBJECT
public:
	explicit UiManager(IControlCenter*);
	~UiManager();

	QAbstractListModel* users();

public Q_SLOTS:
	void updateUserList();

protected:
	IControlCenter* m_cc;
	UserList* m_usersList;

};

}

#endif
