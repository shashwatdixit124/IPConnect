#include "clientmanager.h"

#include "interfaces/iclientmanager.h"
#include "interfaces/iconnection.h"
#include "interfaces/iclient.h"

#include <QObject>
#include <QList>

namespace IPConnect
{

ClientManager::ClientManager(QObject* parent) : IClientManager(parent)
{
}

ClientManager::~ClientManager()
{
}

QList<IClient*> ClientManager::getAllClients() 
{
	return QList<IClient*>();
}

void ClientManager::removeClient(IClient*) 
{
}

void ClientManager::removeClient(qint16) 
{
}

void ClientManager::removeAllClients() 
{
}

void ClientManager::addConnection(IConnection*) 
{
}

void ClientManager::refresh() 
{
}

void ClientManager::closeConnection(IClient*) 
{
}

void ClientManager::closeAllConections() 
{
}

}
