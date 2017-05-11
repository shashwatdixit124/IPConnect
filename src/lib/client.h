#ifndef CLIENT_H
#define CLIENT_H

#include "interfaces/iclient.h"
#include "clientinformation.h"

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

	IConnection* connection() override;
	ClientInformation info() override;

	void setConnection(Connection*);
	void setInfo(ClientInformation);

protected:
	Connection* m_conn;
	ClientInformation m_info;

};

}

#endif
