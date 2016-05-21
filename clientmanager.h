#ifndef CLIENTMANAGER_H
#define CLIENTMANAGER_H

#include <QDebug>
#include <QObject>
#include <QReadWriteLock>
#include <QTcpSocket>
#include <QThread>

#include "client.h"

class ClientManager : public QObject
{
    Q_OBJECT
public:
    explicit ClientManager(QObject *parent = 0);
    ~ClientManager();

    int count();

protected:
    void removeSocket(QTcpSocket *socket);

private:
    QMap<QTcpSocket*, Client*> m_clients;
    QTcpSocket *socket;


protected slots:

    void disconnected();
    void error(QAbstractSocket::SocketError socketError);

public slots:
    void start();
    void quit();
};

#endif // CLIENTMANAGER_H
