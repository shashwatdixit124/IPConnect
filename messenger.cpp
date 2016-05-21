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
    m_manager = new ClientManager();
    qDebug() << this << "Created new Client Manager " << m_manager ;
    m_thread = new QThread(this);
    qDebug() << this << "Created new QThread " << m_thread ;
    connect(this, &Messenger::accepting,m_manager,&ClientManager::accept, Qt::QueuedConnection);
    connect(m_thread,&QThread::started,m_manager,&ClientManager::start, Qt::QueuedConnection);
    connect(m_thread,&QThread::finished,this,&Messenger::threadFinished);
    m_manager->moveToThread(m_thread);
    qDebug() << this << "Client Manager moved to thread " << m_thread;
    m_thread->start();
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
    m_client = new Client();
    m_client->setSocketDescriptor(m_socket->socketDescriptor());
    m_client->moveToThread(m_thread);
    qDebug() << m_client << "moved to thread " << m_thread;
    emit accepting(m_socket->socketDescriptor(),connection,true);
}

void Messenger::serverDestroyed()
{
    qDebug() << this << "Server Destroyed";
    m_thread->deleteLater();
}

void Messenger::threadFinished()
{
    qDebug() << this << "Thread finished " << m_thread;
}

void Messenger::on_actionConnect_triggered()
{
    qDebug() << this << "connect Clicked";
    m_connectDialog = new ConnectDialog(this);
}
