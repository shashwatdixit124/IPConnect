#include "messenger.h"
#include "ui_messenger.h"

#include <QMessageBox>

Messenger::Messenger(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Messenger)
{
    ui->setupUi(this);
    m_MyUsername = "Shashwat";
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
    connect(client,&Client::capturedDetail,this,&Messenger::updateList, Qt::QueuedConnection);
    connect(client,&Client::gotMessageRequest,this,&Messenger::handleMessage, Qt::QueuedConnection);
    connect(client,&Client::warning,this,&Messenger::displayWarning,Qt::QueuedConnection);
    connect(client,&Client::question,this,&Messenger::displayQuestion,Qt::QueuedConnection);
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
    connect(this, &Messenger::sendMessageRequest,m_manager,&ClientManager::sendMessage, Qt::QueuedConnection);
    m_manager->moveToThread(m_thread);
    qDebug() << this << "Client Manager moved to thread " << m_thread;
    m_thread->start();
    if(this->m_server.listen(QHostAddress::Any, 2424))
        qDebug() << this << "Server started  ";
    else
        qDebug() << this << "Server Could not start";
}

void Messenger::readyRead()
{
    qDebug() << this << "incomming Data ";
}

void Messenger::bytesWritten(qint64 bytes)
{
    qDebug() << this << "bytes written = " << bytes << " through socket " << m_socket ;
}

void Messenger::displayWarning(QString t_title, QString t_message)
{
    QMessageBox::warning(this,t_title,t_message);
}

void Messenger::displayQuestion(QString t_title, QString t_filename, QString t_username, qint64 t_size)
{
    Q_UNUSED(t_size);
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this,t_title,"Accept "+t_filename+" from "+t_username,QMessageBox::Yes | QMessageBox::No);
}

void Messenger::handleConnection()
{
    qDebug() << this << "incomming Connection";
    m_socket = m_server.nextPendingConnection();
    m_client = new Client();
    connect(m_client,&Client::destroyingConnection,this,&Messenger::removeUser,Qt::QueuedConnection);
    connect(m_client,&Client::capturedDetail,this,&Messenger::updateList, Qt::QueuedConnection);
    connect(m_client,&Client::gotMessageRequest,this,&Messenger::handleMessage, Qt::QueuedConnection);
    connect(m_client,&Client::warning,this,&Messenger::displayWarning,Qt::QueuedConnection);
    connect(m_client,&Client::question,this,&Messenger::displayQuestion,Qt::QueuedConnection);
    m_client->setSocket(m_socket);
    m_client->setUsername(m_MyUsername);
    m_client->moveToThread(m_thread);
    qDebug() << m_client << "moved to thread " << m_thread;
    emit accepting(m_socket->socketDescriptor(),m_client,true);
}

void Messenger::handleMessage(QString t_message)
{
    qDebug() << this << "Handling Message " << t_message ;
    ui->textEdit->append(t_message.trimmed());
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

void Messenger::updateList(QString t_detail, Client *t_client)
{
    qDebug() << this << "Got the Details " << t_detail;
    ui->listWidget->addItem(t_detail);
    m_users.insert(t_detail,t_client);
}

void Messenger::on_actionConnect_triggered()
{
    qDebug() << this << "connect Clicked";
    m_connectDialog = new ConnectDialog(this);
    connect(m_connectDialog,&ConnectDialog::connect,this,&Messenger::connectManually);
}

void Messenger::on_lineEdit_returnPressed()
{
    QString message = ui->lineEdit->text();
    qDebug() << "return pressed data is " << message;
    if(!message.isEmpty())
    {
        int row = ui->listWidget->currentRow();
        if(row>=0)
        {
            qDebug() << this << "user row is " << row;
            QListWidgetItem *user = ui->listWidget->item(row);
            qDebug() << this << "user is " << user;
            QString username = user->text();
            qDebug() << this << "user selected " << username;
            if(m_users.contains(username))
            {
                qDebug() << this << "value of username is " << m_users[username];

                if(m_users[username])
                {
                    emit sendMessageRequest(message, m_users[username]);
                    ui->textEdit->append(m_MyUsername+" : "+message);
                    ui->lineEdit->setText("");
                }
                else
                {
                    qDebug() << ui->listWidget->currentItem();
                    QMessageBox msgBox;
                    msgBox.setText("User Disconnected");
                    msgBox.exec();
                }
            }
        }
        else
        {
            QMessageBox msgBox;
            msgBox.setText("Select User to send Message");
            msgBox.exec();
        }
    }
}

