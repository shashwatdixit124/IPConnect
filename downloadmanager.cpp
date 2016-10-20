#include "downloadmanager.h"

DownloadManager::DownloadManager(QObject *parent) : QObject(parent)
{
    qDebug() << this << "started download manager";
    connect(&m_server,&QTcpServer::newConnection,this,&DownloadManager::handleTransfer);
    connect(&m_server,&QTcpServer::destroyed,this,&DownloadManager::serverDestroyed);
    if(this->m_server.listen(QHostAddress::Any, 2423))
        qDebug() << this << "Server started on 2423";
    else
        qDebug() << this << "Server Could not start on 2423";
}

DownloadManager::~DownloadManager()
{

}

void DownloadManager::handleTransfer()
{
    qDebug() << this << "new transfer";
    m_socket = m_server.nextPendingConnection();
    qDebug() << this << "got the connection " << m_socket;
}

void DownloadManager::serverDestroyed()
{
    qDebug() << this << "server Destroyed";
}
