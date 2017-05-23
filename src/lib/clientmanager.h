#ifndef CLIENTMANAGER_H
#define CLIENTMANAGER_H

#include "interfaces/iclientmanager.h"
#include "messageinformation.h"

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
	QList<MessageInformation> messages() override;
	void addConnection(IConnection*) override;
	void sendMessage(qint16,QString) override;

Q_SIGNALS:
	void clientCreated(Client*);
	void removeAllClients();
	void sendToClient(qint16,QString);

public Q_SLOTS:
	void clientAdded(ClientInformation);
	void addMessage(qint16,QString);
	void clientRemoved(qint16);

protected:
	Client* createClient(IConnection*);

	QMap<qint16,ClientInformation> m_clientsInfo;
	QMap<qint16,MessageInformation> m_messages;
	qint16 m_clientCount;
	qint16 m_messageCount;
	QThread *m_clientThread;
	ClientThreadManager *m_clientThreadManager;

};

}

#endif
