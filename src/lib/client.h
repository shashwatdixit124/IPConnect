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
	void sendMessage(QString ) override;

	void setConnection(Connection*);
	void setInfo(ClientInformation);
	void start();

public Q_SLOTS:
	void handleRead();
	void closeConnection();

Q_SIGNALS:
	void infoRecieved(ClientInformation);
	void connectionClosed();

protected:
	void processRead(QByteArray);
	void handleRequest();
	void write(QString);
	void sendDetail();

	Connection* m_conn;
	ClientInformation m_info;
	QMap<QString,QString> m_request;
	QMap<QString,QString> m_response;
	bool m_detailAccepted;
	bool m_detailSent;
};

}

#endif
