#ifndef ICLIENTMANAGER_H
#define ICLIENTMANAGER_H

#include <QObject>
#include <QList>

namespace IPConnect
{

class IServer;
class IConnection;
class IClient;

class IClientManager : public QObject
{
	Q_OBJECT

public:
	virtual QList<IClient*> getAllClients() = 0;
	virtual void removeClient(IClient*) = 0;
	virtual void removeClient(qint16) = 0;
	virtual void removeAllClients() = 0;
	virtual void addConnection(IConnection*) = 0;
	virtual void refresh() = 0;

protected:
	virtual void closeConnection(IClient*) = 0;
	virtual void closeAllConections() = 0;
	
	explicit IClientManager(QObject* parent = nullptr);
	~IClientManager();

};

}

#endif
