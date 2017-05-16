#ifndef UIMANAGER_H
#define UIMANAGER_H

#include <QObject>
#include <QQmlApplicationEngine>

namespace IPConnect
{
class IControlCenter;

class UiManager : public QObject
{
	Q_OBJECT
public:
	explicit UiManager(IControlCenter*);
	~UiManager();
	void load(QString);

protected:
	IControlCenter* m_cc;
	QQmlApplicationEngine m_engine;

};

}

#endif
