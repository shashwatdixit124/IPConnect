#ifndef CLIENTMANAGER_H
#define CLIENTMANAGER_H

#include "interfaces/iclientmanager.h"

#include <QObject>
#include <QList>
#include <QMap>
#include <QThread>

namespace IPConnect
{

class IConnection;
class Client;
class ClientInformation;
class ClientThreadManager;

class ClientManager : public IClientManager
{
	Q_OBJECT

public:
	explicit ClientManager(QObject* parent = nullptr);
	~ClientManager();

	void shutdown() override;
	QList<ClientInformation> clients() override;
	void addConnection(IConnection*) override;

Q_SIGNALS:
	void clientCreated(Client*);
	void removeAllClients();

public Q_SLOTS:
	void clientAdded(ClientInformation);
	void clientRemoved(qint16);

protected:
	Client* createClient(IConnection*);

	QMap<qint16,Client*> m_clientList;
	QMap<qint16,ClientInformation> m_clientsInfo;
	qint16 m_clientCount;
	QThread *m_clientThread;
	ClientThreadManager *m_clientThreadManager;

};

}

#endif
