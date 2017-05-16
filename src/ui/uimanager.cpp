#include "uimanager.h"

#include <interfaces/icontrolcenter.h>
#include <interfaces/iusersettings.h>
#include "uimanager.h"

#include <QObject>
#include <QQmlApplicationEngine>
#include <QQmlContext>

namespace IPConnect
{

UiManager::UiManager(IControlCenter* cc) : m_cc(cc)
{
	m_engine.rootContext()->setContextProperty("ipcUserSettings",m_cc->userSettings());
}

UiManager::~UiManager()
{
}

void UiManager::load(QString file)
{
	m_engine.load(QUrl(file));
}

}
