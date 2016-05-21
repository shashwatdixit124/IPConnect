#include "clientmanager.h"

ClientManager::ClientManager(QObject *parent) : QObject(parent)
{
    qDebug() << this << "Client Manager created on thread" << QThread::currentThread();
}

ClientManager::~ClientManager()
{
    qDebug() << this << "Client Manager Destroyed";
}

int ClientManager::count()
{
    QReadWriteLock lock;
    lock.lockForRead();
    int value = m_clients.count();
    lock.unlock();
    return value;
}

void ClientManager::removeSocket(QTcpSocket *socket)
{
    if(!socket) return;
    if(!m_clients.contains(socket)) return;

    qDebug() << this << "removing socket = " <<  socket;

    if(socket->isOpen())
    {
        qDebug() << this << "socket is open, attempting to close it " << socket;
        socket->disconnect();
        socket->close();
    }

    qDebug() << this << "deleting socket" << socket;
    m_clients[socket]->deleteLater();
    m_clients.remove(socket);
    socket->deleteLater();
    qDebug() << this << "client count = " << m_clients.count();
}

void ClientManager::disconnected()
{
    if(!sender()) return;
    qDebug() << this << "disconnecting socket"<< sender();

    QTcpSocket *socket = static_cast<QTcpSocket*>(sender());
    if(!socket) return;

    removeSocket(socket);
}

void ClientManager::error(QAbstractSocket::SocketError socketError)
{
    if(!sender()) return;
    qDebug() << this << "error in socket" << sender() << " error = " << socketError;

    QTcpSocket *socket = static_cast<QTcpSocket*>(sender());
    if(!socket) return;

    removeSocket(socket);
}

void ClientManager::start()
{
    qDebug() << this << "Client Manager started on" << QThread::currentThread();
}

void ClientManager::quit()
{
    if(!sender()) return;
    qDebug() << this << "connections quitting";

    foreach(QTcpSocket *socket, m_clients.keys())
    {
        removeSocket(socket);
    }
}
