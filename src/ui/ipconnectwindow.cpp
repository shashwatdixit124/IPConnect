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

IPConnectWindow::IPConnectWindow()
{
	qmlRegisterSingletonType<UiManager>("api.ui.ipconnect",2,0,"IPConnect",UiManager::uimanager_singleton);
}

IPConnectWindow::~IPConnectWindow()
{
}

void IPConnectWindow::show()
{
	m_engine.load(QUrl("qrc:/resources/MainWindow.qml"));
}

}
