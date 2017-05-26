#ifndef UIMANAGER_H
#define UIMANAGER_H

#include <clientinformation.h>

#include <QObject>
#include <QAbstractListModel>
#include <QPointer>
#include <QQmlEngine>
#include <QJSEngine>

namespace IPConnect
{
class ControlCenter;
class UserList;
class Messenger;
class MessageList;
class IUserSettings;

class UiManager : public QObject
{
	Q_OBJECT
	Q_PROPERTY(QAbstractListModel* users READ users)
	Q_PROPERTY(QAbstractListModel* messages READ messages)

public:
	UiManager();
	~UiManager();

	QAbstractListModel* users();
	QAbstractListModel* messages();

	Q_INVOKABLE void sendMessage(int,QString);

	static QObject* uimanager_singleton(QQmlEngine *engine, QJSEngine *scriptEngine);

protected:
	ControlCenter* m_cc;
	QPointer<UserList> m_usersList;
	QPointer<Messenger> m_messenger;
	QPointer<MessageList> m_messages;
	QPointer<IUserSettings> m_settings;

};

}

#endif
