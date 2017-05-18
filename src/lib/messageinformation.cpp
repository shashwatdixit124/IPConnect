#include "messageinformation.h"

#include <QObject>

namespace IPConnect
{

MessageInformation::MessageInformation(){}

MessageInformation::~MessageInformation(){}

qint16 MessageInformation::sent() const
{
	return m_sent;
}

QString MessageInformation::user() const
{
	return m_user;
}

QString MessageInformation::msg() const
{
	return m_msg;
}

void MessageInformation::setSent(bool sent)
{
	m_sent = sent;
}

void MessageInformation::setUser(QString name)
{
	m_user = name;
}

void MessageInformation::setMsg(QString msg)
{
	m_msg = msg;
}

}

