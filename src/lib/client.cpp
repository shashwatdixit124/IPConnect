#include "client.h"

#include "interfaces/iclient.h"
#include "interfaces/iconnection.h"
#include "connection.h"
#include "clientinformation.h"

#include <QObject>

namespace IPConnect
{

Client::Client(QObject* parent) : IClient(parent)
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
	m_conn = conn;
}

void Client::setInfo(ClientInformation info)
{
	m_info = info;
}

}
