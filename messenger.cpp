#include "messenger.h"
#include "ui_messenger.h"

Messenger::Messenger(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Messenger)
{
    ui->setupUi(this);

    connect(&m_server,&QTcpServer::newConnection,this,&Messenger::handleConnection);
    connect(&m_server,&QTcpServer::destroyed,this,&Messenger::serverDestroyed);

    startServer();
}

Messenger::~Messenger()
{
    qDebug() << this << "Messenger Destroyed";
    delete ui;
}

void Messenger::startServer()
{
    if(this->m_server.listen(QHostAddress::Any, 2424))
    {
        qDebug() << this << "Server started  ";
    }
    else
    {
        qDebug() << this << "Server Could not start";
    }

    m_thread = new QThread(this);
    qDebug() << this << "Created new QThread " << m_thread ;
}

void Messenger::readyRead()
{
    qDebug() << this << "incomming Data ";
}

void Messenger::bytesWritten(qint64 bytes)
{
    qDebug() << this << "bytes written = " << bytes << " through socket " << m_socket ;
}

void Messenger::handleConnection()
{
    qDebug() << this << "incomming Connection";
    m_socket = m_server.nextPendingConnection();
    connect(m_socket,&QTcpSocket::readyRead,this,&Messenger::readyRead,Qt::QueuedConnection);
    connect(m_socket,&QTcpSocket::bytesWritten,this,&Messenger::bytesWritten,Qt::QueuedConnection);
    m_socket->moveToThread(m_thread);
    qDebug() << m_socket << "moved to thread " << m_thread;
}

void Messenger::serverDestroyed()
{
    qDebug() << this << "Server Destroyed";
    m_thread->deleteLater();
}

void Messenger::on_actionConnect_triggered()
{
    qDebug() << this << "connect Clicked";
    m_connectDialog = new ConnectDialog(this);
}
