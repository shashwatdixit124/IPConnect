#ifndef CLIENT_H
#define CLIENT_H

#include "interfaces/iclient.h"

#include <QObject>

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

	void setConnection(Connection* conn);
	IConnection* getConnection();
	void setName(QString name);
	QString name();
	void setIp(QString ip);
	QString ip();

protected:
	Connection* m_conn;
	QString m_name;
	QString m_ip;

};

}

#endif
