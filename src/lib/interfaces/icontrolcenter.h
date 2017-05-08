#ifndef ICONTROLCENTER_H
#define ICONTROLCENTER_H

#include <QObject>

namespace IPConnect
{

class IServer;

class IControlCenter : public QObject
{
	Q_OBJECT

public:
	static IControlCenter* instance();

	virtual IPConnect::IServer* server() = 0;

protected:
	static IControlCenter* m_instance;
	explicit IControlCenter(QObject* parent = nullptr);
};

}

#endif
