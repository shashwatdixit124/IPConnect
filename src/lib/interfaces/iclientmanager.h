#ifndef ICLIENTMANAGER_H
#define ICLIENTMANAGER_H

#include <QObject>
#include <QList>

namespace IPConnect
{

class IConnection;
class ClientInformation;

class IClientManager : public QObject
{
	Q_OBJECT

public:
	virtual void shutdown() = 0;
	virtual QList<ClientInformation> clients() = 0;
	virtual void removeClient(qint16) = 0;
	virtual void removeAllClients() = 0;
	virtual void addClient(IConnection*) = 0;
	virtual void refresh() = 0;

protected:
	explicit IClientManager(QObject* parent = nullptr);
	~IClientManager();

};

}

#endif
