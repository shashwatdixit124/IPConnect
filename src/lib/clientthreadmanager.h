#ifndef CLIENTTHREADMANAGER_H
#define CLIENTTHREADMANAGER_H

#include <QObject>
#include <QMap>

namespace IPConnect
{
class Client;
class ClientInformation;

class ClientThreadManager : public QObject
{
	Q_OBJECT
public:
	explicit ClientThreadManager(QObject* parent = nullptr);
	~ClientThreadManager();

Q_SIGNALS:
	void clientAdded(ClientInformation);
	void clientRemoved(qint16);

public Q_SLOTS:
	void clientCreated(Client*);
	void addClient(ClientInformation);
	void closeConnection(Client*);
	void removeAllClients();
	void removeClient();

protected:
	QMap<qint16,Client*> m_clientList;

};

}

#endif
