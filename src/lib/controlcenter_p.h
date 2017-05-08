#ifndef CONTROLCENTERPRIVATE_H
#define CONTROLCENTERPRIVATE_H

#include "controlcenter.h"

namespace IPConnect
{
class Server;

class ControlCenterPrivate
{
public:
	explicit ControlCenterPrivate(ControlCenter* parent = nullptr);
	void init();
	void shutdown();

	Server* m_server;
	ControlCenter* m_cc;	
};

}

#endif
