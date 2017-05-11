#ifndef CLIENTMANAGER_H
#define CLIENTMANAGER_H

#include "interfaces/iclientmanager.h"

#include <QObject>
#include <QList>
#include <QMap>

namespace IPConnect
{

class IConnection;
class Client;
class ClientInformation;

class ClientManager : public IClientManager
{
	Q_OBJECT

public:
	explicit ClientManager(QObject* parent = nullptr);
	~ClientManager();

	void shutdown() override;
	QList<ClientInformation> clients() override;
	void removeClient(qint16) override;
	void removeAllClients() override;
	void addClient(IConnection*) override;
	void refresh() override;

protected:
	void closeConnection(Client*);
	void closeAllConections();

	QMap<qint16,Client*> m_clientList;
	QList<ClientInformation> m_clients;
	qint16 m_clientCount;

};

}

#endif
