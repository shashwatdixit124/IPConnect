#include "client.h"

#include "interfaces/iclient.h"
#include "interfaces/iconnection.h"
#include "connection.h"
#include "clientinformation.h"
#include "debug.h"

#include <QObject>

namespace IPConnect
{

Client::Client(QObject* parent) : IClient(parent) , m_conn(nullptr)
{
}

Client::~Client()
{
}

IConnection* Client::connection()
{
	return m_conn;
}

ClientInformation Client::info()
{
	return m_info;
}

void Client::setConnection(Connection* conn)
{
	if(m_conn)
		disconnect(m_conn,&Connection::readyRead,this,&Client::handleRead);
	m_conn = conn;
	connect(m_conn,&Connection::readyRead,this,&Client::handleRead);
}

void Client::setInfo(ClientInformation info)
{
	m_info = info;
}

void Client::start()
{
	qCDebug(BASE) << "client started on " << thread() ;
}

void Client::handleRead()
{
	qCDebug(BASE) << "Reading from Connection(" << m_conn->socketDescriptor() << ") => " << m_conn->readAll() ;
}

}
