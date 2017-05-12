#ifndef CLIENT_H
#define CLIENT_H

#include "interfaces/iclient.h"
#include "clientinformation.h"

#include <QObject>
#include <QMap>

namespace IPConnect
{

class IConnection;
class Connection;

class Client : public IClient
{
	Q_OBJECT

public:
	explicit Client(QObject* parent = nullptr);
	~Client();

	IConnection* connection() override;
	ClientInformation info() override;

	void setConnection(Connection*);
	void setInfo(ClientInformation);
	void start();

public Q_SLOTS:
	void handleRead();

Q_SIGNALS:
	void infoRecieved(ClientInformation);

protected:
	void processRead(QByteArray);
	void handleRequest();
	void write(QString);

	Connection* m_conn;
	ClientInformation m_info;
	QMap<QString,QString> m_request;
	QMap<QString,QString> m_response;
	bool m_detailAccepted;
	bool m_detailSent;
};

}

#endif
