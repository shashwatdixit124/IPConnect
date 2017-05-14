#include "controlcenter.h"

#include "interfaces/iclientmanager.h"
#include "interfaces/icontrolcenter.h"
#include "interfaces/iserver.h"
#include "interfaces/iusersettings.h"
#include "clientmanager.h"
#include "controlcenter_p.h"
#include "server.h"
#include "usersettings.h"

#include <QObject>

namespace IPConnect
{

ControlCenter* ControlCenter::m_instance = nullptr;

ControlCenter::ControlCenter(QObject* parent) : IControlCenter(parent) , m_ccp(new ControlCenterPrivate(this)){}

ControlCenter::~ControlCenter()
{
	shutdown();
}

ControlCenter* ControlCenter::instance()
{
	return m_instance;
}

void ControlCenter::init()
{
	m_instance = new ControlCenter();
	m_instance->m_ccp->init();
}

void ControlCenter::quit()
{
	m_instance->deleteLater();
	m_instance = nullptr;
}

IServer* ControlCenter::server()
{
	return m_ccp->m_server;
}

IClientManager* ControlCenter::clientManager()
{
	return m_ccp->m_clientManager;
}

IUserSettings* ControlCenter::userSettings()
{
	return m_ccp->m_userSettings;
}

void ControlCenter::shutdown()
{
	m_instance->m_ccp->shutdown();
	delete m_ccp;
}

}
