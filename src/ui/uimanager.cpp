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

#include "uimanager.h"

#include <controlcenter.h>
#include <interfaces/icontrolcenter.h>
#include <interfaces/iclientmanager.h>
#include <interfaces/iusersettings.h>
#include "userlist.h"
#include "messenger.h"
#include "messagelist.h"

#include <QAbstractListModel>
#include <QQmlEngine>
#include <QJSEngine>

namespace IPConnect
{

UiManager::UiManager() : m_cc(ControlCenter::instance()) , m_selectedUser(-1)
{
	IClientManager* cm = m_cc->clientManager();
	m_usersList = new UserList(cm);
	m_messenger = new Messenger(cm);
	m_messages = new MessageList(cm);
	m_settings = m_cc->userSettings();
}

UiManager::~UiManager()
{
}

QAbstractListModel* UiManager::users()
{
	return m_usersList;
}

QAbstractListModel* UiManager::messages()
{
	return m_messages;
}

bool UiManager::runningFirstTime()
{
	return m_settings->runningFirstTime();
}

QString UiManager::theme()
{
	return m_settings->theme();
}

void UiManager::setTheme(QString color)
{
	if(color != theme())
		m_settings->setTheme(color);
	emit themeChanged();
}

QString UiManager::userName()
{
	return m_settings->name();
}

void UiManager::setUserName(QString name)
{
	if(name != userName() && !name.isEmpty() && !name.isNull())
		m_settings->setName(name);
	emit userNameChanged();
}

QString UiManager::downloadDir()
{
	return m_settings->downloadDir();
}

void UiManager::setDownloadDir(QString ddir)
{
	if(ddir != downloadDir() && !ddir.isEmpty() && !ddir.isNull())
		m_settings->setDownloadDir(ddir);
	emit downloadDirChanged();
}

qint16 UiManager::selectedUser()
{
	return m_selectedUser;
}

void UiManager::setSelectedUser(qint16 id)
{
	m_selectedUser = id;
	emit selectedUserChanged();
}

void UiManager::sendMessage(QString msg)
{
	m_messenger->sendMessage(m_selectedUser,msg);
}

void UiManager::quickConnect(QString url)
{
	if(url.isEmpty() || url.isNull())
		return;

	m_cc->clientManager()->connectManualy(url);
}

QObject* UiManager::uimanager_singleton(QQmlEngine *engine, QJSEngine *scriptEngine)
{
	Q_UNUSED(engine)
	Q_UNUSED(scriptEngine)

	UiManager *manager = new UiManager();
	return manager;
}

}
