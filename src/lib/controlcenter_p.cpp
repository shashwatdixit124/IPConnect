#include "controlcenter_p.h" 

#include "clientmanager.h"
#include "controlcenter.h"
#include "server.h"
#include "usersettings.h"

namespace IPConnect
{

ControlCenterPrivate::ControlCenterPrivate(ControlCenter* parent) : m_cc(parent){}

ControlCenterPrivate::~ControlCenterPrivate(){}

void ControlCenterPrivate::init()
{
	m_userSettings = new UserSettings();
	m_clientManager = new ClientManager(m_cc);
	m_server = new Server(m_cc);
	m_server->start();
}

void ControlCenterPrivate::shutdown()
{
	if(m_server)
		m_server->deleteLater();

	if(m_clientManager)
		m_clientManager->deleteLater();

	if(m_userSettings)
		m_userSettings->deleteLater();
}

}
