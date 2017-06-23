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
class PendingTransfers;
class RunningTransfers;
class IClientManager;
class IUserSettings;
class ITransferManager;

/**
 * @brief Singleton to Connect ControlCenter to GUI
 * provides all information and methods to GUI
 * @author Shashwat Dixit <shashwatdixit124@gmail.com>
 */
class UiManager : public QObject
{
	Q_OBJECT
	Q_PROPERTY(QAbstractListModel* users READ users CONSTANT)
	Q_PROPERTY(QAbstractListModel* messages READ messages CONSTANT)
	Q_PROPERTY(QAbstractListModel* pendingTransfers READ pendingTransfers CONSTANT)
	Q_PROPERTY(QAbstractListModel* runningTransfers READ runningTransfers CONSTANT)
	Q_PROPERTY(bool runningFirstTime READ runningFirstTime CONSTANT)
	Q_PROPERTY(QString theme READ theme WRITE setTheme NOTIFY themeChanged)
	Q_PROPERTY(QString userName READ userName WRITE setUserName NOTIFY userNameChanged)
	Q_PROPERTY(QString downloadDir READ downloadDir WRITE setDownloadDir NOTIFY downloadDirChanged)
	Q_PROPERTY(qint16 selectedUser READ selectedUser WRITE setSelectedUser NOTIFY selectedUserChanged)
	Q_PROPERTY(QString notificationMsg READ notificationMsg WRITE setNotificationMsg NOTIFY notificationMsgChanged)
	Q_PROPERTY(QString notificationStatus READ notificationStatus WRITE setNotificationStatus NOTIFY notificationStatusChanged)
	Q_PROPERTY(int unreadMessages READ unreadMessages WRITE setUnreadMessages NOTIFY unreadMessagesChanged)
	Q_PROPERTY(int unseenTransfers READ unseenTransfers WRITE setUnseenTransfers NOTIFY unseenTransfersChanged)

public:
	UiManager();
	~UiManager();

	/**
	 * @brief list of all client
	 * @return UserList
	 */
	QAbstractListModel* users();

	/**
	 * @brief list of all messages
	 * @return MessageList
	 */
	QAbstractListModel* messages();

	/**
	 * @brief list of pending Trasnfers
	 * @return PendingTransfers
	 */
	QAbstractListModel* pendingTransfers();

	/**
	 * @brief list of running transfers
	 * @return RunningTransfers
	 */
	QAbstractListModel* runningTransfers();
	
	/**
	 * @brief checks if running first time
	 * @return true if running first time, false otherwise
	 */
	bool runningFirstTime();

	QString theme();
	void setTheme(QString color);

	QString userName();
	void setUserName(QString name);

	QString downloadDir();
	void setDownloadDir(QString ddir);

	qint16 selectedUser();
	void setSelectedUser(qint16 id);

	QString notificationMsg();
	void setNotificationMsg(QString msg);

	QString notificationStatus();
	void setNotificationStatus(QString status);

	int unreadMessages();
	void setUnreadMessages(int);

	int unseenTransfers();
	void setUnseenTransfers(int m);

	Q_INVOKABLE void sendMessage(QString);
	Q_INVOKABLE void sendFile(QString);
	Q_INVOKABLE void quickConnect(QString);
	Q_INVOKABLE void acceptTransfer(qint16);
	Q_INVOKABLE void rejectTransfer(qint16);
	Q_INVOKABLE void stopTransfer(qint16);

	static QObject* uimanager_singleton(QQmlEngine *engine, QJSEngine *scriptEngine);

protected Q_SLOTS:
	void manualConnectionFailed(QString);
	void messagesAdded(int);
	void transfersAdded(int);

Q_SIGNALS:
	void themeChanged();
	void userNameChanged();
	void downloadDirChanged();
	void selectedUserChanged();
	void notificationMsgChanged();
	void notificationStatusChanged();
	void unreadMessagesChanged();
	void unseenTransfersChanged();

protected:
	ControlCenter* m_cc;
	QPointer<UserList> m_usersList;
	QPointer<Messenger> m_messenger;
	QPointer<MessageList> m_messages;
	QPointer<PendingTransfers> m_pendingTransfersList;
	QPointer<RunningTransfers> m_runningTransfersList;
	IClientManager* m_clientManager;
	IUserSettings* m_settings;
	ITransferManager* m_transferManager;
	QString m_notificationMsg;
	QString m_notificationStatus;
	qint16 m_selectedUser;
	int m_unreadMessages;
	int m_unseenTransfers;

};

}

#endif
