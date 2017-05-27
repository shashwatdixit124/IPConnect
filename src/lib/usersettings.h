#ifndef USERSETTINGS_H
#define USERSETTINGS_H

#include "interfaces/iusersettings.h"

#include <QObject>
#include <QSettings>

namespace IPConnect
{

class UserSettings : public IUserSettings
{
	Q_OBJECT

public:
	explicit UserSettings();
	~UserSettings();

	QString ip() override;
	bool runningFirstTime() override;
	QString name() override;
	QString downloadDir() override;
	QString theme() override;
	void setName(QString) override;
	void setDownloadDir(QString) override;
	void setTheme(QString) override;

protected:
	QString m_name;
	QString m_ip;
	QString m_downloadDir;
	QString m_theme;
	bool m_set;
	bool m_first;
	QSettings m_config;

};

}

#endif
