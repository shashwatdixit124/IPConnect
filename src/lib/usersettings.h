#ifndef USERSETTINGS_H
#define USERSETTINGS_H

#include "interfaces/iusersettings.h"

#include <QObject>

namespace IPConnect
{

class UserSettings : public IUserSettings
{
	Q_OBJECT

public:
	explicit UserSettings();
	~UserSettings();

	QString name() override;
	QString ip() override;
	QString downloadDir() override;
	void setName(QString) override;
	void setDownloadDir(QString) override;

protected:
	QString m_name;
	QString m_ip;
	QString m_downloadDir;
	bool m_set;

};

}

#endif
