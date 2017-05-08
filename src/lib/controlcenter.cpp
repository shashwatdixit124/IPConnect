#include "interfaces/icontrolcenter.h"
#include "interfaces/iserver.h"

#include "controlcenter.h"
#include "server.h"

namespace IPConnect
{

ControlCenter::ControlCenter(QObject* parent) : IControlCenter(parent)
{
	m_server = nullptr;
}

~ControlCenter::ControlCenter()
{
	shutdown();
}

void ControlCenter::init()
{
	m_instance = new ControlCenter();
	m_server = new Server(m_instance);
}

IControlCenter* ControlCenter::instance()
{
	return m_instance;
}

IServer* ControlCenter::server()
{
	return m_server;
}

void ControlCenter::shutdown()
{
	if(m_server)
		m_server->deleteLater();
}

}
