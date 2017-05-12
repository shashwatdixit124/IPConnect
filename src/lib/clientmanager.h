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
	void addConnection(IConnection*) override;
	void refresh() override;

public Q_SLOTS:
	void addClient(ClientInformation);

protected:
	Client* createClient(IConnection*);
	void closeConnection(Client*);
	void closeAllConections();

	QMap<qint16,Client*> m_clientList;
	QMap<qint16,ClientInformation> m_clientsInfo;
	qint16 m_clientCount;
	QThread *m_clientThread ;

};

}

#endif
