#ifndef IUSERSETTINGS_H
#define IUSERSETTINGS_H

#include <QObject>

namespace IPConnect
{

class IUserSettings : public QObject
{
	Q_OBJECT

public:
	Q_INVOKABLE virtual QString name() = 0;
	Q_INVOKABLE virtual QString ip() = 0;
	Q_INVOKABLE virtual QString downloadDir() = 0;
	Q_INVOKABLE virtual void setName(QString) = 0;
	Q_INVOKABLE virtual void setDownloadDir(QString) = 0;

protected:
	explicit IUserSettings();
	~IUserSettings();

};

}

#endif
