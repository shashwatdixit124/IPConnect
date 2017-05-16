#include "ipconnectwindow.h"

#include "interfaces/icontrolcenter.h"
#include "uimanager.h"

#include <QQmlApplicationEngine>
#include <QObject>

namespace IPConnect
{

IPConnectWindow::IPConnectWindow(IControlCenter* cc) : m_manager(new UiManager(cc))
{
}

IPConnectWindow::~IPConnectWindow()
{
}

void IPConnectWindow::show()
{
	m_engine.load(QUrl("qrc:/resources/MainWindow.qml"));
}

}
