#include "clientmanager.h"

#include "interfaces/iclientmanager.h"
#include "interfaces/iconnection.h"
#include "interfaces/iclient.h"
#include "client.h"
#include "connection.h"
#include "clientinformation.h"
#include "clientthreadmanager.h"
#include "debug.h"

#include <QObject>
#include <QList>
#include <QThread>

namespace IPConnect
{

ClientManager::ClientManager(QObject* parent)
	: IClientManager(parent) , m_clientCount(0) , 
	m_clientThread(new QThread(this)) , m_clientThreadManager(new ClientThreadManager())
{
	qCDebug(BASE) << "ClientManager started";
	connect(m_clientThreadManager,&ClientThreadManager::clientAdded,this,&ClientManager::clientAdded,Qt::QueuedConnection);
	connect(m_clientThreadManager,&ClientThreadManager::clientRemoved,this,&ClientManager::clientRemoved,Qt::QueuedConnection);
	connect(this,&ClientManager::clientCreated,m_clientThreadManager,&ClientThreadManager::clientCreated,Qt::QueuedConnection);
	connect(this,&ClientManager::removeAllClients,m_clientThreadManager,&ClientThreadManager::removeAllClients,Qt::QueuedConnection);
	m_clientThreadManager->moveToThread(m_clientThread);
	m_clientThread->start();
}

ClientManager::~ClientManager()
{
}

void ClientManager::shutdown()
{
	emit removeAllClients();
	m_clientThread->quit();
	m_clientThread->deleteLater();
	qCDebug(BASE) << "ClientManager Stopped" ;
}

QList<ClientInformation> ClientManager::clients()
{
	return m_clientsInfo.values();
}

void ClientManager::clientRemoved(qint16 id)
{
	m_clientList.remove(id);
	m_clientsInfo.remove(id);
	emit userListUpdated();
}

void ClientManager::addConnection(IConnection* connection)
{
	qCDebug(BASE) << this << " adding new Client with descriptor " <<  connection->socketDescriptor();
	Client* client = createClient(connection);

	if(!client){
		qCDebug(BASE) << "cannot add Client with socket descriptor " << connection->socketDescriptor() ;
		return;
	}

	connection->moveToThread(m_clientThread);
	client->moveToThread(m_clientThread);
	emit clientCreated(client);
}

void ClientManager::clientAdded(ClientInformation ci)
{
	qint16 id = ci.id();
	Client* client = m_clientList.value(id);
	client->setInfo(ci);
	m_clientsInfo.insert(id,ci);
	qCDebug(BASE) << this << ci.name() << " Added in ClientManager" ;
	emit userListUpdated();
}

Client* ClientManager::createClient(IConnection* connection)
{
	Connection *conn = dynamic_cast<Connection*>(connection);
	if(!conn){
		connection->deleteLater();
		return nullptr;
	}

	Client* client = new Client();
	client->setConnection(conn);

	m_clientCount++;
	m_clientList.insert(m_clientCount,client);
	ClientInformation ci;
	ci.setId(m_clientCount);
	client->setInfo(ci);
	return client;
}

}
