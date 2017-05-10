#ifndef CONTROLCENTER_H
#define CONTROLCENTER_H

#include "interfaces/icontrolcenter.h"

#include <QObject>

namespace IPConnect
{

class IServer;
class IClientManager;
class ControlCenterPrivate;

class ControlCenter : public IControlCenter
{
	Q_OBJECT
public:
	explicit ControlCenter(QObject* parent = nullptr);
	~ControlCenter();
	static ControlCenter* instance();
	static void init();

	IServer* server() override;
	IClientManager* clientManager() override;

public Q_SLOTS:
	void shutdown();

protected:
	friend ControlCenterPrivate;
	IPConnect::ControlCenterPrivate* m_ccp;
	static ControlCenter* m_instance;

};

}

#endif
