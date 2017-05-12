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
	connect(m_clientThread,&QThread::finished,this,&ClientManager::shutdown,Qt::QueuedConnection);
	m_clientThread->start();
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
	Client* client = new Client();
	Connection *conn = dynamic_cast<Connection*>(connection);
	if(!conn){
		qCDebug(BASE) << "cannot add Client with socket descriptor " << connection->socketDescriptor() ;
		connection->deleteLater();
		return;
	}
	client->setConnection(conn);

	m_clientCount++;
	m_clientList.insert(m_clientCount,client);
	ClientInformation ci;
	ci.setId(m_clientCount);
	client->setInfo(ci);

	connect(client,&Client::infoRecieved,this,&ClientManager::addClient,Qt::QueuedConnection);
	connection->moveToThread(m_clientThread);
	client->moveToThread(m_clientThread);
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
