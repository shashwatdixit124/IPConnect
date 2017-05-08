#ifndef ICONTROLCENTER_H
#define ICONTROLCENTER_H

#include <QObject>
#include <QTcpServer>

namespace IPConnect
{

class IServer : public QObject
{
	Q_OBJECT

public:
	virtual start() = 0;

protected:
	QTcpServer m_server;
	explicit IServer(QObject* parent);
};

}

#endif
