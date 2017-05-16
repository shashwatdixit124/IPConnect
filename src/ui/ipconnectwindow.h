#ifndef IPCONNECTWINDOW_H
#define IPCONNECTWINDOW_H

#include <QQmlApplicationEngine>

namespace IPConnect
{
class IControlCenter;
class UiManager;

class IPConnectWindow
{

public:
	explicit IPConnectWindow(IControlCenter* cc);
	~IPConnectWindow();
	void show();

protected:
	UiManager* m_manager;
	QQmlApplicationEngine m_engine;
};

}

#endif
