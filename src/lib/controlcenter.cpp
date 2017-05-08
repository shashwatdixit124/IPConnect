#include "interfaces/icontrolcenter.h"
#include "interfaces/iserver.h"
#include "controlcenter.h"
#include "controlcenter_p.h"
#include "server.h"

#include <QObject>

namespace IPConnect
{

ControlCenter::ControlCenter(QObject* parent) : IControlCenter(parent) , m_ccp(new ControlCenterPrivate(this))
{

}

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

IServer* ControlCenter::server()
{
	return m_ccp->m_server;
}

void ControlCenter::shutdown()
{
	m_instance->m_ccp->shutdown();
}

}
