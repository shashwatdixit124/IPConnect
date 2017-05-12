#include "client.h"

#include "interfaces/iclient.h"
#include "interfaces/iconnection.h"
#include "interfaces/iusersettings.h"
#include "connection.h"
#include "controlcenter.h"
#include "clientinformation.h"
#include "debug.h"

#include <QObject>

namespace IPConnect
{

Client::Client(QObject* parent) : IClient(parent) , m_conn(nullptr) , m_detailAccepted(false) , m_detailSent(false)
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
	qCDebug(BASE) << "Client started on " << thread() ;
	sendDetail();
}

void Client::handleRead()
{
	QByteArray data = m_conn->readAll();
	qCDebug(BASE) << "Reading from Connection(" << m_conn->socketDescriptor() << ") => " << data ;
	processRead(data);
}

void Client::processRead(QByteArray t_data)
{
	QString header = t_data;
	qCDebug(BASE) << this << "Processing GET header = "<<header;
	m_request.insert("request",header);

	//Header has 4 values APP METHOD OPTION DATA(optional)
	QStringList options = header.split(":");

	if(options.count() >= 1) m_request.insert("app", options.at(0).trimmed());

	if(options.count() >= 2) m_request.insert("method", options.at(1).trimmed());

	if(options.count() >= 3) m_request.insert("option", options.at(2).trimmed());

	if(options.count() >= 4)
	{
		int pos = header.indexOf(":") + m_request.value("method","").size() + m_request.value("option","").size() + 2;
		m_request.insert("data", header.mid(pos + 1));
	}

	handleRequest();
}

void Client::handleRequest()
{
	QString method = m_request.value("method","");
	QString option = m_request.value("option","");

	if(method == "CONNECT")
	{
		m_request.insert("message",m_request.value("request"));
		if(option == "REQUEST")
		{
			if(!m_detailAccepted)
			{
				if(m_request.value("option") == "REQUEST")
				{
					qCDebug(BASE) << this << "Accepting Detail";
					QString data = m_request.value("data");
					QString ip = data.split(":").at(0);
					int pos = data.indexOf(":");
					QString name = data.mid(pos + 1);
					m_info.setName(name.trimmed());
					m_info.setIp(ip.trimmed());
					qCDebug(BASE) << this << "Accepted request from "<< name;

					if(!m_detailSent)
					{
						QString myIp = ControlCenter::instance()->userSettings()->ip();
						QString myName = ControlCenter::instance()->userSettings()->name();
						m_response.insert("app","IPC");
						m_response.insert("method","CONNECT");
						m_response.insert("option","REQUEST");
						m_response.insert("data",myIp+":"+myName);
						QString message = "IPC:CONNECT:REQUEST:"+myIp+":"+myName;
						m_response.insert("message",message);
						m_detailSent = true;
						write(message);
					}
					m_detailAccepted = true;
					emit infoRecieved(m_info);
				}
			}
		}
	}
	return;
}

void Client::write(QString t_message)
{
	if(m_conn){
		m_conn->write(t_message.toUtf8());
		m_conn->waitForBytesWritten();
	}
	else
		qCDebug(BASE) << this << "Connection not available on write";
}

void Client::sendDetail()
{
	if(m_conn)
	{
		qCDebug(BASE) << this << "Sending Details" ;
		m_detailSent = true;
		QString myIp = ControlCenter::instance()->userSettings()->ip();
		QString myName = ControlCenter::instance()->userSettings()->name();
		QString message = "IPC:CONNECT:REQUEST:"+myIp+":"+myName;
		m_response.insert("app","IPC");
		m_response.insert("method","CONNECT");
		m_response.insert("option","REQUEST");
		m_response.insert("data",myIp+":"+myName);
		m_response.insert("message",message);
		write(message);
	}
	else
		qCDebug(BASE) << this << "Connection not available on greetings";
}

}
