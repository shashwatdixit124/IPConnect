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
}

void Messenger::readyRead()
{
    qDebug() << this << "incomming Data through socket " << m_socket;
    m_socket->write(m_socket->readAll());
}

void Messenger::bytesWritten(qint64 bytes)
{
    qDebug() << this << "bytes written = " << bytes << " through socket " << m_socket ;
}

void Messenger::handleConnection()
{
    qDebug() << this << "incomming Connection";
    m_socket = m_server.nextPendingConnection();
    connect(m_socket,&QTcpSocket::readyRead,this,&Messenger::readyRead);
    connect(m_socket,&QTcpSocket::bytesWritten,this,&Messenger::bytesWritten);
}

void Messenger::serverDestroyed()
{
    qDebug() << this << "Server Destroyed";
}
