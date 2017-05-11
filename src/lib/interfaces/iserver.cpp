#include "iserver.h"

#include <QTcpServer>

namespace IPConnect
{

IServer::IServer(QObject *parent) : QTcpServer(parent){}

IServer::~IServer(){}

}
