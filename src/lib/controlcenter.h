#ifndef CONTROLCENTER_H
#define CONTROLCENTER_H

#include "interfaces/icontrolcenter.h"

#include <QObject>

namespace IPConnect
{

class IServer;
class IClientManager;
class ControlCenterPrivate;
class IUserSettings;

class ControlCenter : public IControlCenter
{
	Q_OBJECT
public:
	explicit ControlCenter(QObject* parent = nullptr);
	~ControlCenter();
	static ControlCenter* instance();
	static void init();
	static void quit();

	IServer* server() override;
	IClientManager* clientManager() override;
	IUserSettings* userSettings() override;

public Q_SLOTS:
	void shutdown();

protected:
	friend ControlCenterPrivate;
	IPConnect::ControlCenterPrivate* m_ccp;
	static ControlCenter* m_instance;

};

}

#endif
