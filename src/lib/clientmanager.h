#ifndef CLIENTMANAGER_H
#define CLIENTMANAGER_H

#include "interfaces/iclientmanager.h"

#include <QObject>
#include <QList>
#include <QMap>

namespace IPConnect
{

class IConnection;
class IClient;
class Client;

class ClientManager : public IClientManager
{
	Q_OBJECT

public:
	explicit ClientManager(QObject* parent = nullptr);
	~ClientManager();

	QList<IClient*> getAllClients() override;
	void removeClient(IClient*) override;
	void removeClient(qint16) override;
	void removeAllClients() override;
	void addConnection(IConnection*) override;
	void refresh() override;

protected:
	void closeConnection(IClient*) override;
	void closeAllConections() override;

	QMap<QString,Client*> m_clientList;

};

}

#endif
