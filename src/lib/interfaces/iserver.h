#ifndef ISERVER_H
#define ISERVER_H

#include <QObject>
#include <QTcpServer>

namespace IPConnect
{

class IServer : public QObject
{
	Q_OBJECT

public:
	virtual void start() = 0;

protected:
	explicit IServer(QObject* parent);
	~IServer();
};

}

#endif
