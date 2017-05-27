#include "usersettings.h"

#include <QHostAddress>
#include <QNetworkInterface>
#include <QSettings>

namespace IPConnect
{

UserSettings::UserSettings() : m_name(QString("Pilot")) , m_theme(QString("#16a085")) , m_downloadDir(QString("~/Downloads"))
{
	int count = m_config.value("Count").toInt();
	m_config.setValue("Count",count + 1);

	m_first = count == 0;

	if(!m_first)
	{
		m_name = m_config.value("UserName").toString();
		m_theme = m_config.value("ColorTheme").toString();
		m_downloadDir = m_config.value("DownloadDir").toString();
	}
	else
	{
		m_config.setValue("UserName",m_name);
		m_config.setValue("ColorTheme",m_theme);
		m_config.setValue("DownloadDir",m_downloadDir);
	}

	QString ip;
	QList<QHostAddress> list = QNetworkInterface::allAddresses();
	for(int nIter=0; nIter<list.count(); nIter++)
	{
		if(!list[nIter].isLoopback())
			if (list[nIter].protocol() == QAbstractSocket::IPv4Protocol )
				ip = list[nIter].toString();
	}
	m_ip = ip;
}

UserSettings::~UserSettings()
{
}

QString UserSettings::ip()
{
	return m_ip;
}

bool UserSettings::runningFirstTime()
{
	return m_first;
}

QString UserSettings::name()
{
	return m_name;
}

QString UserSettings::downloadDir()
{
	return m_downloadDir;
}

QString UserSettings::theme()
{
	return m_theme;
}

void UserSettings::setName(QString name)
{
	m_name = name;
	m_config.setValue("UserName",name);
}

void UserSettings::setDownloadDir(QString dd)
{
	m_downloadDir = dd;
	m_config.setValue("DownloadDir",dd);
}

void UserSettings::setTheme(QString theme)
{
	m_theme = theme;
	m_config.setValue("ColorTheme",theme);
}

}
