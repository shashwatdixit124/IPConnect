#include "usersettings.h"

#include <QHostAddress>
#include <QNetworkInterface>

namespace IPConnect
{

UserSettings::UserSettings() : m_set(false)
{
	if(!m_set){
		m_set = true;
		m_name = QString("Shashwat");
		m_downloadDir = QString("~/Downloads");
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

QString UserSettings::name()
{
	return m_name;
}

QString UserSettings::ip()
{
	return m_ip;
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
}

void UserSettings::setDownloadDir(QString dd)
{
	m_downloadDir = dd;
}

void UserSettings::setTheme(QString theme)
{
	m_theme = theme;
}

}
