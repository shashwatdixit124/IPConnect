#include "clientinformation.h"

#include <QObject>

namespace IPConnect
{

ClientInformation::ClientInformation(){}

ClientInformation::~ClientInformation(){}

qint16 ClientInformation::id() const
{
	return m_id;
}

QString ClientInformation::name() const
{
	return m_name;
}

QString ClientInformation::ip() const
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
