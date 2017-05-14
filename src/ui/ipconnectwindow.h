#ifndef IPCONNECTWINDOW_H
#define IPCONNECTWINDOW_H

#include "interfaces/icontrolcenter.h"

#include <QQmlApplicationEngine>

namespace IPConnect
{

class IPConnectWindow
{

public:
	explicit IPConnectWindow(IControlCenter* cc);
	~IPConnectWindow();
	void show();

protected:
	QQmlApplicationEngine m_engine;
	IControlCenter* m_cc;
};

}

#endif
