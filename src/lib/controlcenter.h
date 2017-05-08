#ifndef CONTROLCENTER_H
#define CONTROLCENTER_H

#include "interfaces/icontrolcenter.h"

#include <QObject>

namespace IPConnect
{

class Server;
class IServer;
class ControlCenterPrivate;

class ControlCenter : public IControlCenter
{
public:
	explicit ControlCenter(QObject* parent = nullptr);
	~ControlCenter();
	static ControlCenter* instance();
	static void init();

	IServer* server() override;

public Q_SLOTS:
	void shutdown();

protected:
	friend class ControlCenterPrivate;
	ControlCenterPrivate* m_ccp;
	static ControlCenter* m_instance;

};

}

#endif
