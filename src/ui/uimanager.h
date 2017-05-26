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
	Q_PROPERTY(QString theme READ theme WRITE setTheme NOTIFY themeChanged)
	Q_PROPERTY(QString userName READ userName WRITE setUserName NOTIFY userNameChanged)
	Q_PROPERTY(QString downloadDir READ downloadDir WRITE setDownloadDir NOTIFY downloadDirChanged)

public:
	UiManager();
	~UiManager();

	QAbstractListModel* users();
	QAbstractListModel* messages();

	QString theme();
	void setTheme(QString);

	QString userName();
	void setUserName(QString);

	QString downloadDir();
	void setDownloadDir(QString);

	Q_INVOKABLE void sendMessage(int,QString);
	Q_INVOKABLE void quickConnect(QString);

	static QObject* uimanager_singleton(QQmlEngine *engine, QJSEngine *scriptEngine);

Q_SIGNALS:
	void themeChanged();
	void userNameChanged();
	void downloadDirChanged();

protected:
	ControlCenter* m_cc;
	QPointer<UserList> m_usersList;
	QPointer<Messenger> m_messenger;
	QPointer<MessageList> m_messages;
	QPointer<IUserSettings> m_settings;

};

}

#endif
