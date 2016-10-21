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
    qDebug() << this << "destroyed";
}

void DownloadManager::handleTransfer()
{
    qDebug() << this << "new transfer";
    m_socket = m_server.nextPendingConnection();
    qDebug() << this << "got the connection " << m_socket;
}

void DownloadManager::readyRead()
{
    QString data = m_socket->readAll();
    qDebug() << this << data;
}

void DownloadManager::bytesWritten(qint64 t_bytes)
{
    qDebug() << this << "written " << t_bytes;
}

void DownloadManager::serverDestroyed()
{
    qDebug() << this << "server Destroyed";
}

void DownloadManager::transfer(QString t_address, QString t_filename, QString t_filepath, bool t_isSending)
{
    if(!t_isSending)
    {
        m_socket = new QTcpSocket();
        connect(m_socket,&QTcpSocket::readyRead,this,&DownloadManager::readyRead);
        connect(m_socket,&QTcpSocket::bytesWritten,this,&DownloadManager::bytesWritten);
        m_socket->connectToHost(t_address,2423);
        if(m_socket->isOpen() && !t_address.isEmpty() && !t_filename.isEmpty() && !t_filepath.isEmpty())
        {
            m_transfer = new Transfer;
            m_transfer->m_ip = t_address;
            m_transfer->m_filename = t_filename;
            m_transfer->m_filepath = t_filepath;
            m_transfer->isSending = t_isSending;
            m_transfers.insert(t_filename,m_transfer);
            m_socket->write("IPC:FILE:TRANSFER:"+t_filename.toUtf8());
        }
        else
        {
            qDebug() << this << "something wrong with connection" ;
            m_socket->deleteLater();
        }
    }
    else
    {
        m_transfer = new Transfer;
        m_transfer->m_ip = t_address;
        m_transfer->m_filename = t_filename;
        m_transfer->m_filepath = t_filepath;
        m_transfer->isSending = t_isSending;
        m_transfers.insert(t_filename,m_transfer);
    }
}
