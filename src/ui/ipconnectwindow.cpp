#include "ipconnectwindow.h"

#include <interfaces/icontrolcenter.h>
#include <interfaces/iusersettings.h>
#include "uimanager.h"
#include "messenger.h"

#include <QObject>
#include <QQmlApplicationEngine>
#include <QAbstractListModel>
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
	m_engine.rootContext()->setContextProperty("_users",m_manager->users());
	m_engine.rootContext()->setContextProperty("_messenger",m_manager->messenger());
	m_engine.rootContext()->setContextProperty("_messages",m_manager->messages());
	m_engine.rootContext()->setContextProperty("_settings",m_manager->settings());
	m_engine.load(QUrl("qrc:/resources/MainWindow.qml"));
}

}
