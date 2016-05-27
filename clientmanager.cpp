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

void ClientManager::sendMessage(QString t_message, Client* t_client)
{
    try{
        t_client->sendMessage(t_message);
    }catch(...)
    {
        qDebug()<<this<<"NO User";
    }
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
    emit finished();
}

void ClientManager::sendFile(QString t_file, Client *t_client)
{
    try{
        t_client->requestSendFile(t_file);
    }catch(...)
    {
        qDebug()<<"NO User";
    }
}

void ClientManager::accept(qintptr t_handle, Client *t_client, bool t_server)
{
    qDebug() << this << "accepting for a client for server = "<<t_server;
    QTcpSocket *socket = new QTcpSocket(this);

    if(!socket->setSocketDescriptor(t_handle))
    {
        qWarning() << this << "could not accept connection" << t_handle;
        t_client->deleteLater();
        return;
    }

    connect(socket,&QTcpSocket::disconnected, this, &ClientManager::disconnected);
    connect(socket,static_cast<void (QTcpSocket::*)(QAbstractSocket::SocketError)>(&QTcpSocket::error),this,&ClientManager::error);

    t_client->moveToThread(QThread::currentThread());
    t_client->setSocket(socket);

    m_clients.insert(socket,t_client);
    qDebug() << this << "clients = " << m_clients.count();
    if(t_server)
        t_client->sendDetail();
    emit socket->connected();
}

void ClientManager::connectToHost(Client *t_client, QString t_addr, int t_port)
{
    qDebug() << this << "inside connect to host";
    m_socket = new QTcpSocket();
    t_client->setSocket(m_socket);
    t_client->moveToThread(QThread::currentThread());
    qDebug() << this << "socket is " << t_client->getSocket();
    connect(m_socket,&QTcpSocket::disconnected, this, &ClientManager::disconnected);
    connect(m_socket,static_cast<void (QTcpSocket::*)(QAbstractSocket::SocketError)>(&QTcpSocket::error),this,&ClientManager::error);

    m_socket->connectToHost(t_addr,t_port);
    qDebug() << this << "socketDescriptor of manual connection = " << t_client->getSocket()->socketDescriptor() ;
    m_clients.insert(m_socket,t_client);
    qDebug() << this << "clients = " << m_clients.count();
    emit m_socket->connected();
}
