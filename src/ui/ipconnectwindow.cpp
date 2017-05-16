#include "ipconnectwindow.h"

#include "interfaces/icontrolcenter.h"
#include "uimanager.h"

#include <QObject>
#include <QQmlApplicationEngine>
#include <QQmlContext>

namespace IPConnect
{

IPConnectWindow::IPConnectWindow(IControlCenter* cc) : m_manager(new UiManager(cc))
{
}

IPConnectWindow::~IPConnectWindow()
{
	if(m_manager)
		m_manager->deleteLater();
}

void IPConnectWindow::show()
{
	m_engine.rootContext()->setContextProperty("_ipconnect",m_manager);
	m_engine.load(QUrl("qrc:/resources/MainWindow.qml"));
}

}
