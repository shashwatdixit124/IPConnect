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
	explicit IPConnectWindow();
	~IPConnectWindow();
	void show();

protected:
	QQmlApplicationEngine m_engine;
};

}

#endif
