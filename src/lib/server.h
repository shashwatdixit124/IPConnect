#ifndef SERVER_H
#define SERVER_H

#include "interfaces/iserver.h"

#include <QObject>

namespace IPConnect
{

class Server : public IServer
{
	Q_OBJECT
public:
	explicit Server(QObject* parent);
	~Server();
	void start() override;

protected:
	QTcpServer m_server;
};

}

#endif
 
