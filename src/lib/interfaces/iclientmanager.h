#ifndef ICLIENTMANAGER_H
#define ICLIENTMANAGER_H

#include <QObject>
#include <QList>

namespace IPConnect
{

class IConnection;
class ClientInformation;

class IClientManager : public QObject
{
	Q_OBJECT

public:
	virtual void shutdown() = 0;
	virtual QList<ClientInformation> clients() = 0;
	virtual void addConnection(IConnection*) = 0;
	virtual void sendMessage(qint16,QString) = 0;

Q_SIGNALS:
	void userListUpdated();

protected:
	explicit IClientManager(QObject* parent = nullptr);
	~IClientManager();

};

}

#endif
