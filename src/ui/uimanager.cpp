#include "uimanager.h"

#include <interfaces/icontrolcenter.h>
#include <interfaces/iclientmanager.h>
#include <clientinformation.h>
#include "userlist.h"
#include "messenger.h"

#include <QAbstractListModel>

namespace IPConnect
{

UiManager::UiManager(IControlCenter* cc) : m_cc(cc)
{
	IClientManager* cm = cc->clientManager();
	m_usersList = new UserList(cm->clients());
	connect(cm,&IClientManager::userListUpdated,this,&UiManager::updateUserList);

	m_messenger = new Messenger(cm);
}

UiManager::~UiManager()
{
	if(m_usersList)
		m_usersList->deleteLater();

	if(m_messenger)
		m_messenger->deleteLater();
}

void UiManager::updateUserList()
{
	m_usersList->updateList(m_cc->clientManager()->clients());
}

QAbstractListModel* UiManager::users()
{
	return m_usersList;
}

Messenger* UiManager::messenger()
{
	return m_messenger;
}

}
