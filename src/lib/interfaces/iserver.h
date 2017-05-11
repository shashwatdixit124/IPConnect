#ifndef ISERVER_H
#define ISERVER_H

#include <QObject>
#include <QTcpServer>

namespace IPConnect
{

class IServer : public QTcpServer
{
	Q_OBJECT

public:
	virtual void start() = 0;

protected:
	explicit IServer(QObject* parent);
	~IServer();
	virtual void incomingConnection(qintptr handle) Q_DECL_OVERRIDE {};

};

}

#endif
