#ifndef IUSERSETTINGS_H
#define IUSERSETTINGS_H

#include <QObject>

namespace IPConnect
{

class IUserSettings : public QObject
{
	Q_OBJECT

public:
	virtual QString name() = 0;
	virtual QString ip() = 0;
	virtual QString downloadDir() = 0;
	virtual void setName(QString) = 0;
	virtual void setDownloadDir(QString) = 0;

protected:
	explicit IUserSettings();
	~IUserSettings();

};

}

#endif
