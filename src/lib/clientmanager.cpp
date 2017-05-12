#include "clientmanager.h"

#include "interfaces/iclientmanager.h"
#include "interfaces/iconnection.h"
#include "interfaces/iclient.h"
#include "client.h"
#include "connection.h"
#include "clientinformation.h"
#include "debug.h"

#include <QObject>
#include <QList>
#include <QThread>

namespace IPConnect
{

ClientManager::ClientManager(QObject* parent) : IClientManager(parent) , m_clientCount(0) , m_clientThread(new QThread(this))
{
	qCDebug(BASE) << "ClientManager started on " << thread() ;
	connect(m_clientThread,&QThread::finished,this,&ClientManager::shutdown,Qt::QueuedConnection);
	m_clientThread->start();
	qCDebug(BASE) << "Starting Client Thread " << m_clientThread ;
}

ClientManager::~ClientManager()
{
}

void ClientManager::shutdown()
{
	removeAllClients();
	m_clientThread->terminate();
	m_clientThread->deleteLater();
}

QList<ClientInformation> ClientManager::clients()
{
	return m_clientsInfo.values();
}

void ClientManager::removeClient(qint16 id)
{
	Client* client = m_clientList.value(id);
	m_clientList.remove(id);
	m_clientsInfo.remove(id);
	closeConnection(client);
	client->deleteLater();
}

void ClientManager::removeAllClients()
{
	QList<Client*> clients = m_clientList.values();
	foreach(Client* c , clients){
		closeConnection(c);
		c->deleteLater();
	}
	m_clientsInfo.clear();
}

void ClientManager::addConnection(IConnection* connection)
{
	qCDebug(BASE) << "adding new Client with descriptor " <<  connection->socketDescriptor();
	Client* client = createClient(connection);

	if(!client){
		qCDebug(BASE) << "cannot add Client with socket descriptor " << connection->socketDescriptor() ;
		return;
	}

	connect(client,&Client::infoRecieved,this,&ClientManager::addClient,Qt::QueuedConnection);
	connection->moveToThread(m_clientThread);
	client->moveToThread(m_clientThread);
	client->start();
}

void ClientManager::refresh()
{
}

void ClientManager::addClient(ClientInformation ci)
{
	qint16 id = ci.id();
	Client* client = m_clientList.value(id);
	client->setInfo(ci);
	m_clientsInfo.insert(id,ci);
	emit clientAdded(ci);
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
	foreach(Client* c , clients){
		closeConnection(c);
	}
}

}
