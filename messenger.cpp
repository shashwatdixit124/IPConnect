#include "messenger.h"
#include "ui_messenger.h"

Messenger::Messenger(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Messenger)
{
    ui->setupUi(this);
    m_MyUsername = "test";
    connect(&m_server,&QTcpServer::newConnection,this,&Messenger::handleConnection);
    connect(&m_server,&QTcpServer::destroyed,this,&Messenger::serverDestroyed);

    startServer();
}

Messenger::~Messenger()
{
    qDebug() << this << "Messenger Destroyed";
    if(m_thread->isRunning())
    {
        m_thread->deleteLater();
        m_thread->quit();
    }
    delete ui;
}

void Messenger::connectManually(QString address)
{
    qDebug() << this << "Connect Manually Called with address " << address;

    Client* client = new Client();
    connect(client,&Client::destroyingConnection,this,&Messenger::removeUser,Qt::QueuedConnection);
    client->setUsername(m_MyUsername);
    client->moveToThread(m_thread);

    emit connectToHost(client,address,2424);
    qDebug() << this << "connected to host" ;
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
    connect(m_manager,&ClientManager::finished,m_manager,&ClientManager::quit,Qt::QueuedConnection);
    connect(this,&Messenger::connectToHost,m_manager,&ClientManager::connectToHost,Qt::QueuedConnection);
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
    m_client->setSocket(m_socket);
    m_client->moveToThread(m_thread);
    qDebug() << m_client << "moved to thread " << m_thread;
    emit accepting(m_socket->socketDescriptor(),m_client,true);
}

void Messenger::removeUser(QString t_username)
{
    if(t_username == "")
        return;
    if(m_users.contains(t_username))
    {
        m_users.remove(t_username);
        qDebug() << this << "Removed "<<t_username<<" from m_users";
    }

    QList<QListWidgetItem*> list =  ui->listWidget->selectedItems();
    foreach (QListWidgetItem *item, list) {
        delete item;
    }
    qDebug() << this << "Removed "<<t_username<<" from list";
}

void Messenger::serverDestroyed()
{
    qDebug() << this << "Server Destroyed";
    if(m_thread->isRunning())
    {
        m_thread->deleteLater();
        m_thread->quit();
    }
}

void Messenger::threadFinished()
{
    qDebug() << this << "Thread finished " << m_thread;
}

void Messenger::on_actionConnect_triggered()
{
    qDebug() << this << "connect Clicked";
    m_connectDialog = new ConnectDialog(this);
    connect(m_connectDialog,&ConnectDialog::connect,this,&Messenger::connectManually);
}

void Messenger::updateList(QString t_detail, Client *t_client)
{
    qDebug() << this << "Got the Details " << t_detail;
    ui->listWidget->addItem(t_detail);
    m_users.insert(t_detail,t_client);
}
