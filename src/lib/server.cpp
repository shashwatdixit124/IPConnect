#include "server.h"
#include "debug.h"

namespace IPConnect
{

Server::Server(QObject* parent) : IServer(parent)
{

}

void Server::start()
{

    if(m_server.listen(QHostAddress::Any, 2424))
		qCDebug(BASE) << this << "Server started on 2424";
    else
		qCDebug(BASE) << this << "Server Could not start on 2424";
}

}
