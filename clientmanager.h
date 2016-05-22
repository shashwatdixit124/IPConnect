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
    QTcpSocket *m_socket;

signals:
    void finished();

protected slots:
    void disconnected();
    void error(QAbstractSocket::SocketError socketError);

public slots:
    void accept(qintptr, Client *, bool );
    void connectToHost(Client*, QString, int);
    void quit();
    void start();
};

#endif // CLIENTMANAGER_H
