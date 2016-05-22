#include "client.h"

Client::Client(QObject *parent) : QObject(parent)
{
    qDebug() << this << "Client Created";
}

Client::~Client()
{
    qDebug() << this << "Client Destroyed";
    emit destroyingConnection(m_ClientUsername);
}

const QTcpSocket *Client::getSocket()
{
    return m_socket;
}

void Client::setClientUsername(const QString t_username)
{
    m_ClientUsername = t_username;
}

void Client::sendDetail()
{
    qDebug() << this << "Checking for sockets";
    if(m_socket)
    {
        QByteArray message = "IPC:CONNECT:REQUEST:"+m_MyUsername.toUtf8();
        m_socket->write(message);
    }
    else
        qDebug() << this << "socket not available on greetings";
}

void Client::setSocket(QTcpSocket *t_socket)
{
    m_socket = t_socket;
    connect(m_socket,&QTcpSocket::connected, this, &Client::connected,Qt::DirectConnection);
    connect(m_socket,&QTcpSocket::disconnected, this, &Client::disconnected,Qt::DirectConnection);
    connect(m_socket,&QTcpSocket::readyRead, this, &Client::readyRead,Qt::DirectConnection);
    connect(m_socket,&QTcpSocket::bytesWritten, this, &Client::bytesWritten,Qt::DirectConnection);
    connect(m_socket,&QTcpSocket::stateChanged, this, &Client::stateChanged,Qt::DirectConnection);
    connect(m_socket,static_cast<void (QTcpSocket::*)(QAbstractSocket::SocketError)>(&QTcpSocket::error),this,&Client::error,Qt::DirectConnection);
}

void Client::setUsername(const QString t_username)
{
    m_MyUsername = t_username;
}

void Client::connected()
{
    qDebug() << this << "Client Connected";
}

void Client::disconnected()
{
    qDebug() << this << "Client Disconnected";
}

void Client::readyRead()
{
    qDebug() << this << "ReadyRead signaled => " << m_socket->readAll();
}

void Client::bytesWritten(qint64 bytes)
{
    qDebug() << this << "BytesWritten Called " << bytes;
}

void Client::stateChanged(QAbstractSocket::SocketState socketState)
{
    qDebug() << this << "stateChanged" << m_socket << socketState;
}

void Client::error(QAbstractSocket::SocketError socketError)
{
    qDebug() << this << "error" << m_socket << socketError;
}
