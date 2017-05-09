#include "server.h"

#include "debug.h"

#include <QObject>
#include <QTcpServer>

namespace IPConnect
{

Server::Server(QObject* parent) : IServer(parent){}

Server::~Server(){}

void Server::start()
{
    if(m_server.listen(QHostAddress::Any, 2424))
		qCDebug(BASE) << "Server started on 2424";
    else
		qCDebug(BASE) << "Server Could not start on 2424";
}

}
