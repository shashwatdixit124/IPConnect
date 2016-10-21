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

void DownloadManager::processRead(QString t_data, QTcpSocket* t_socket)
{
    QString header = t_data;
    qDebug() << this << "Processing GET header = "<<header;
    qDebug() << this << "sender =  " << t_socket;
    QStringList options = header.split(":");
    m_request.insert("app", options.at(0).trimmed());
    m_request.insert("method", options.at(1).trimmed());
    m_request.insert("ip", options.at(2).trimmed());
    int pos = header.indexOf(":") + m_request.value("method","").size() + m_request.value("ip","").size() + 2;
    m_request.insert("filename", header.mid(pos + 1));

    if(m_transfers.contains(m_request.value("filename")))
    {
        Transfer *temp = m_transfers.value(m_request.value("filename"));
        if(temp->m_ip == m_request.value("ip"))
        {
            sendFile(temp,t_socket);
        }
    }
}

void DownloadManager::sendFile(Transfer *t_transfer, QTcpSocket *t_socket)
{

    if(!t_socket) return;
    qDebug() << this << "Sending file " << t_transfer->m_filename;

    QFile *file = new QFile(t_transfer->m_filepath,this);
    FileTransfer* fileTransfer = new FileTransfer();

    connect(fileTransfer,&FileTransfer::finished,this, &DownloadManager::fileTransferFinished,Qt::QueuedConnection);

    if(!file->open(QFile::ReadOnly))
    {
        qWarning() << "Could not open file: " << file;
        return;
    }

    fileTransfer->setSource(file);
    fileTransfer->setDestination(t_socket);
    fileTransfer->setRate(1024000);
    fileTransfer->setSize(102400);
    fileTransfer->setSender(true);

    disconnect(t_socket,&QTcpSocket::readyRead,this,&DownloadManager::readyRead);
    disconnect(t_socket,&QTcpSocket::bytesWritten,this,&DownloadManager::bytesWritten);
    QThread *thread = new QThread(this);
    t_socket->moveToThread(thread);
    fileTransfer->moveToThread(thread);
    qDebug() << this << "fileTransfer moved to " << thread;
    m_threads.insert(t_socket,thread);
    QTimer::singleShot(500, fileTransfer, SLOT(start()));
}

void DownloadManager::acceptFile(Transfer *t_transfer, QTcpSocket * t_socket)
{
    if(!t_socket) return;
    qDebug() << this << "Sending file " << t_transfer->m_filename;

    QFile *file = new QFile(t_transfer->m_filepath,this);
    FileTransfer* fileTransfer = new FileTransfer();

    connect(fileTransfer,&FileTransfer::finished,this, &DownloadManager::fileTransferFinished,Qt::QueuedConnection);

    if(!file->open(QFile::WriteOnly))
    {
        qWarning() << "Could not open file: " << t_transfer->m_filepath;
        return;
    }

    fileTransfer->setSource(t_socket);
    fileTransfer->setDestination(file);
    fileTransfer->setRate(1024000);
    fileTransfer->setSize(102400);
    fileTransfer->setSender(false);
    fileTransfer->setFileSize(t_transfer->m_filesize);

    disconnect(t_socket,&QTcpSocket::readyRead,this,&DownloadManager::readyRead);
    disconnect(t_socket,&QTcpSocket::bytesWritten,this,&DownloadManager::bytesWritten);
    QThread *thread = new QThread(this);
    t_socket->moveToThread(thread);
    fileTransfer->moveToThread(thread);
    qDebug() << this << "fileTransfer moved to " << thread;
    m_threads.insert(t_socket,thread);
    QTimer::singleShot(5, fileTransfer, SLOT(start()));
}

void DownloadManager::handleTransfer()
{
    m_socket = m_server.nextPendingConnection();
    connect(m_socket,&QTcpSocket::readyRead,this,&DownloadManager::readyRead);
    connect(m_socket,&QTcpSocket::bytesWritten,this,&DownloadManager::bytesWritten);
    qDebug() << this << "got the connection on 2423" << m_socket;
}

void DownloadManager::readyRead()
{
    if(!sender()) return;
    QString data = m_socket->readAll();
    qDebug() << this << data;
    QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender());
    processRead(data,socket);
}

void DownloadManager::bytesWritten(qint64 t_bytes)
{
    qDebug() << this << "written " << t_bytes;
}

void DownloadManager::serverDestroyed()
{
    qDebug() << this << "server Destroyed";
}

void DownloadManager::transfer(QString t_address, QString t_filename, QString t_filepath, qint64 t_filesize, bool t_isSending)
{
    qDebug() << this << t_address << t_filename << t_filepath << t_filesize <<  t_isSending;
    if(!t_isSending)
    {
        m_socket = new QTcpSocket();
        connect(m_socket,&QTcpSocket::readyRead,this,&DownloadManager::readyRead);
        connect(m_socket,&QTcpSocket::bytesWritten,this,&DownloadManager::bytesWritten);
        m_socket->connectToHost(t_address,2423);
        if(m_socket->isOpen() && !t_address.isEmpty() && !t_filename.isEmpty() && !t_filepath.isEmpty())
        {
            qDebug() << this << "creating new transfer";
            m_transfer = new Transfer;
            m_transfer->m_ip = t_address;
            m_transfer->m_filename = t_filename;
            m_transfer->m_filepath = t_filepath;
            m_transfer->m_filesize = t_filesize;
            m_transfer->isSending = t_isSending;
            m_transfers.insert(t_filename,m_transfer);
            qDebug() << this << "sending transfer request";
            m_socket->write("IPC:TRANSFER:"+t_address.toUtf8()+":"+t_filename.toUtf8());
            acceptFile(m_transfer,m_socket);
        }
        else
        {
            qDebug() << this << "something wrong with connection";
            m_socket->deleteLater();
        }
    }
    else
    {
        qDebug() << this << "creating new transfer";
        m_transfer = new Transfer;
        m_transfer->m_ip = t_address;
        m_transfer->m_filename = t_filename;
        m_transfer->m_filepath = t_filepath;
        m_transfer->m_filesize = t_filesize;
        m_transfer->isSending = t_isSending;
        m_transfers.insert(t_filename,m_transfer);
    }
}

void DownloadManager::fileTransferFinished()
{
    if(!sender()) return;
    qDebug() << this << "file transfer finished for " << qobject_cast<FileTransfer*>(sender());
}
