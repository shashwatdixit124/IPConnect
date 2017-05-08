#include "controlcenter_p.h" 

#include "controlcenter.h"
#include "server.h"

namespace IPConnect
{

ControlCenterPrivate::ControlCenterPrivate(ControlCenter* parent) : m_cc(parent)
{
	
}

void ControlCenterPrivate::init()
{
	m_server = new Server(m_cc);
	m_server->start();
}

void ControlCenterPrivate::shutdown()
{
	if(m_server)
		m_server->deleteLater();
}

}
