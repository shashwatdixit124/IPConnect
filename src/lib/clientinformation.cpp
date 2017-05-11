#include "clientinformation.h"

#include <QObject>

namespace IPConnect
{

ClientInformation::ClientInformation(){}

ClientInformation::~ClientInformation(){}

qint16 ClientInformation::id()
{
	return m_id;
}

QString ClientInformation::name()
{
	return m_name;
}

QString ClientInformation::ip()
{
	return m_ip;
}

void ClientInformation::setId(qint16 id)
{
	m_id = id;
}

void ClientInformation::setName(QString name)
{
	m_name = name;
}

void ClientInformation::setIp(QString ip)
{
	m_ip = ip;
}

}
