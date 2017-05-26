#include "connection.h"

#include <QAbstractSocket>

namespace IPConnect
{

Connection::Connection() : isInUse(false)
{
	connect(this,static_cast<void (Connection::*)(QAbstractSocket::SocketError)>(&Connection::error),this,&Connection::isError);
}

Connection::~Connection(){}

void Connection::isError()
{
	emit errorOccurred();
}

}
