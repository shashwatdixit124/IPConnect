#include "clientthreadmanager.h"

#include "client.h"
#include "connection.h"
#include "debug.h"
#include "clientinformation.h"

#include <QThread>
#include <QObject>
#include <QMap>

namespace IPConnect
{

ClientThreadManager::ClientThreadManager(QObject* parent) : QObject(parent)
{
}

ClientThreadManager::~ClientThreadManager()
{
}

void ClientThreadManager::clientCreated(Client* client)
{
	qint16 id = client->info().id();
	m_clientList.insert(id,client);
	connect(client,&Client::infoRecieved,this,&ClientThreadManager::addClient);
	connect(client,&Client::connectionClosed,this,&ClientThreadManager::removeClient);
	qCDebug(BASE) << this << "Client Added with id " << id ;
	client->start();
}

void ClientThreadManager::addClient(ClientInformation ci)
{
	qint16 id = ci.id();
	Client* client = m_clientList.value(id);
	client->setInfo(ci);
	emit clientAdded(ci);
}

void ClientThreadManager::removeAllClients()
{
	QList<Client*> clients = m_clientList.values();
	foreach(Client* c , clients){
		closeConnection(c);
		c->deleteLater();
	}
}

void ClientThreadManager::closeConnection(Client* client)
{
	IConnection* conn = client->connection();
	if(conn){
		if(conn->isValid())
			conn->close();
		conn->deleteLater();
	}
}

void ClientThreadManager::removeClient()
{
	if(!sender())
		return;

	Client* client = dynamic_cast<Client*>(sender());
	qint16 id = client->info().id();
	client->deleteLater();
	m_clientList.remove(id);
	emit clientRemoved(id);
}

void ClientThreadManager::sendToClient(qint16 id, QString msg)
{
	Client* client = m_clientList.value(id);
	if(!client)
	{
		qCDebug(BASE) << this << "sending message " << msg << " to client " << id  ;
		return;
	}
	client->sendMessage(msg);
}

}
