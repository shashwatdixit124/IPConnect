#include "ipconnectwindow.h"

//#include "controlcenter.h"
#include "interfaces/icontrolcenter.h"
#include "interfaces/iusersettings.h"

#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QObject>

namespace IPConnect
{

IPConnectWindow::IPConnectWindow(IControlCenter* cc) : m_cc(cc)
{
	m_engine.rootContext()->setContextProperty("ipcUserSettings",m_cc->userSettings());
}

IPConnectWindow::~IPConnectWindow()
{
}

void IPConnectWindow::show()
{
	m_engine.load(QUrl(QLatin1String("qrc:/resources/MainWindow.qml")));
}

}
