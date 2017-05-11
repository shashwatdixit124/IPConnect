#include "clientmanager.h"

#include "interfaces/iclientmanager.h"
#include "interfaces/iconnection.h"
#include "interfaces/iclient.h"
#include "client.h"
#include "clientinformation.h"

#include <QObject>
#include <QList>

namespace IPConnect
{

ClientManager::ClientManager(QObject* parent) : IClientManager(parent) , m_clientCount(0)
{
}

ClientManager::~ClientManager()
{
}

void ClientManager::shutdown()
{
	removeAllClients();
}

QList<ClientInformation> ClientManager::clients()
{
	return m_clients;
}

void ClientManager::removeClient(qint16 id)
{
	Client* client = m_clientList.value(id);
	m_clientList.remove(id);
	closeConnection(client);
	client->deleteLater();
}

void ClientManager::removeAllClients()
{
	QList<Client*> clients = m_clientList.values();
	foreach(Client* c , clients)
	{
		closeConnection(c);
		c->deleteLater();
	}
}

void ClientManager::addClient(IConnection*)
{
}

void ClientManager::refresh()
{
}

void ClientManager::closeConnection(Client* client)
{
	IConnection* conn = client->connection();
	if(conn){
		if(conn->isValid())
			conn->disconnectFromHost();
		conn->deleteLater();
	}
}

void ClientManager::closeAllConections()
{
	QList<Client*> clients = m_clientList.values();
	foreach(Client* c , clients)
	{
		closeConnection(c);
	}
}

}
