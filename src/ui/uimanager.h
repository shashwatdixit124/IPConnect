#ifndef UIMANAGER_H
#define UIMANAGER_H

#include <clientinformation.h>

#include <QObject>
#include <QAbstractListModel>

namespace IPConnect
{
class IControlCenter;
class UserList;
class Messenger;
class MessageList;

class UiManager : public QObject
{
	Q_OBJECT
public:
	explicit UiManager(IControlCenter*);
	~UiManager();

	QAbstractListModel* users();
	Messenger* messenger();
	QAbstractListModel* messages();

protected:
	IControlCenter* m_cc;
	UserList* m_usersList;
	Messenger* m_messenger;
	MessageList* m_messages;

};

}

#endif
