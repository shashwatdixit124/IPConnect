#include "client.h"

#include "interfaces/iclient.h"
#include "interfaces/iconnection.h"
#include "connection.h"

#include <QObject>

namespace IPConnect
{

Client::Client(QObject* parent) : IClient(parent)
{
}

Client::~Client()
{
}

void Client::setConnection(Connection* conn)
{
	m_conn = conn;
}

IConnection* Client::getConnection()
{
	return m_conn;
}

void Client::setName(QString name)
{
	m_name = name;
}

QString Client::name()
{
	return m_name;
}

void Client::setIp(QString ip)
{
	m_ip = ip;
}

QString Client::ip()
{
	return m_ip;
}

}
