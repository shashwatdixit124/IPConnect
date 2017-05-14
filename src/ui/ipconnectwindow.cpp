#include "ipconnectwindow.h"

#include <QQmlApplicationEngine>

namespace IPConnect
{

IPConnectWindow::IPConnectWindow()
{
}

IPConnectWindow::~IPConnectWindow()
{
}

void IPConnectWindow::show()
{
	m_engine.load(QUrl(QLatin1String("qrc:/resources/MainWindow.qml")));
}

}
