#ifndef CONTROLCENTER_H
#define CONTROLCENTER_H

#include <QObject>

namespace IPConnect
{

class Server;
class IControlCenter;
class IServer;

class ControlCenter : public IControlCenter
{
public:
	explicit ControlCenter(QObject* parent = nullptr);
	~ControlCenter();
	static void init();

	static IControlCenter* instance() override;
	IServer* server() override;

public Q_SLOTS:
	void shutdown();

public:
	Server* m_server;
};

}

#endif
