#include "uimanager.h"

#include <interfaces/icontrolcenter.h>
#include <interfaces/iclientmanager.h>
#include "userlist.h"
#include "messenger.h"
#include "messagelist.h"

#include <QAbstractListModel>

namespace IPConnect
{

UiManager::UiManager(IControlCenter* cc) : m_cc(cc)
{
	IClientManager* cm = cc->clientManager();
	m_usersList = new UserList(cm);
	m_messenger = new Messenger(cm);
	m_messages = new MessageList(cm);
}

UiManager::~UiManager()
{
	if(m_usersList)
		m_usersList->deleteLater();

	if(m_messenger)
		m_messenger->deleteLater();

	if(m_messages)
		m_messages->deleteLater();
}

QAbstractListModel* UiManager::users()
{
	return m_usersList;
}

Messenger* UiManager::messenger()
{
	return m_messenger;
}

QAbstractListModel* UiManager::messages()
{
	return m_messages;
}

}
