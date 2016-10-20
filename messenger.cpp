#include "messenger.h"
#include "ui_messenger.h"

Messenger::Messenger(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Messenger)
{
    ui->setupUi(this);
    m_MyUsername = "Shashwat";
    m_DownloadDirectory = "/home/";
    connect(&m_server,&QTcpServer::newConnection,this,&Messenger::handleConnection);
    connect(&m_server,&QTcpServer::destroyed,this,&Messenger::serverDestroyed);
    connect(&m_detailDialog,&DetailDialog::saveClicked,this,&Messenger::defaultChanged);
    m_detailDialog.setDefault(m_MyUsername,m_DownloadDirectory);
    m_detailDialog.exec();
    startServer();
}

Messenger::~Messenger()
{
    if(m_thread->isRunning())
    {
        m_thread->deleteLater();
        m_thread->quit();
    }
    delete ui;
}

void Messenger::connectManually(QString t_address)
{
    Client* client = new Client();
    connect(client,&Client::destroyingConnection,this,&Messenger::removeUser,Qt::QueuedConnection);
    connect(client,&Client::capturedDetail,this,&Messenger::updateList, Qt::QueuedConnection);
    connect(client,&Client::gotMessageRequest,this,&Messenger::handleMessage, Qt::QueuedConnection);
    connect(client,&Client::warning,this,&Messenger::displayWarning,Qt::QueuedConnection);
    connect(client,&Client::question,this,&Messenger::displayQuestion,Qt::QueuedConnection);
    connect(this,&Messenger::fileAccept,client,&Client::fileAccepted,Qt::QueuedConnection);
    connect(this,&Messenger::fileReject,client,&Client::fileRejected,Qt::QueuedConnection);
    client->setDefaultSettings(m_MyUsername,m_DownloadDirectory);
    client->moveToThread(m_thread);
    emit connectToHost(client,t_address,2424);
}

void Messenger::startServer()
{
    m_manager = new ClientManager();
    m_thread = new QThread(this);
    connect(this, &Messenger::accepting,m_manager,&ClientManager::accept, Qt::QueuedConnection);
    connect(m_thread,&QThread::started,m_manager,&ClientManager::start, Qt::QueuedConnection);
    connect(m_thread,&QThread::finished,this,&Messenger::threadFinished);
    connect(this,&Messenger::connectToHost,m_manager,&ClientManager::connectToHost,Qt::QueuedConnection);
    connect(this, &Messenger::sendMessageRequest,m_manager,&ClientManager::sendMessage, Qt::QueuedConnection);
    connect(this, &Messenger::sendFileRequest,m_manager,&ClientManager::sendFile, Qt::QueuedConnection);
    m_manager->moveToThread(m_thread);
    m_thread->start();
    if(this->m_server.listen(QHostAddress::Any, 2424))
        qDebug() << this << "Server started on 2424";
    else
        qDebug() << this << "Server Could not start on 2424";
}

void Messenger::defaultChanged(QString t_directory, QString t_username)
{
    m_DownloadDirectory = t_directory+"/";
    m_MyUsername = t_username;
    qDebug() << this << "Default Changed to " << m_MyUsername << " , " << m_DownloadDirectory;
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
    if(reply == QMessageBox::Yes)
        emit fileAccept();
    else
        emit fileReject();
}

void Messenger::handleConnection()
{
    m_socket = m_server.nextPendingConnection();
    m_client = new Client();
    connect(m_client,&Client::destroyingConnection,this,&Messenger::removeUser,Qt::QueuedConnection);
    connect(m_client,&Client::capturedDetail,this,&Messenger::updateList, Qt::QueuedConnection);
    connect(m_client,&Client::gotMessageRequest,this,&Messenger::handleMessage, Qt::QueuedConnection);
    connect(m_client,&Client::warning,this,&Messenger::displayWarning,Qt::QueuedConnection);
    connect(m_client,&Client::question,this,&Messenger::displayQuestion,Qt::QueuedConnection);
    connect(this,&Messenger::fileAccept,m_client,&Client::fileAccepted,Qt::QueuedConnection);
    connect(this,&Messenger::fileReject,m_client,&Client::fileRejected,Qt::QueuedConnection);
    m_client->setSocket(m_socket);
    m_client->setDefaultSettings(m_MyUsername,m_DownloadDirectory);
    m_client->moveToThread(m_thread);
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
    ui->listWidget->addItem(t_detail);
    m_users.insert(t_detail,t_client);
}

void Messenger::on_actionConnect_triggered()
{
    m_connectDialog = new ConnectDialog(this);
    connect(m_connectDialog,&ConnectDialog::connect,this,&Messenger::connectManually);
}

void Messenger::on_lineEdit_returnPressed()
{
    QString message = ui->lineEdit->text();
    if(!message.isEmpty())
    {
        int row = ui->listWidget->currentRow();
        if(row>=0)
        {
            QListWidgetItem *user = ui->listWidget->item(row);
            QString username = user->text();
            if(m_users.contains(username))
            {
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

void Messenger::on_actionSend_File_triggered()
{
    int row = ui->listWidget->currentRow();
    if(row>=0)
    {
        QListWidgetItem *user = ui->listWidget->item(row);
        QString username = user->text();
        if(m_users.contains(username))
        {
            if(m_users[username])
            {
                QString fileName = QFileDialog::getOpenFileName(this, "Select a file to open...", QDir::homePath());
                qDebug() << this << fileName;
                if(!fileName.isEmpty())
                    emit sendFileRequest(fileName, m_users[username]);
            }
            else
            {
                QList<QListWidgetItem*> list =  ui->listWidget->selectedItems();
                foreach (QListWidgetItem *item, list) {
                    delete item;
                }
                QMessageBox msgBox;
                msgBox.setText("User Disconnected");
                msgBox.exec();
            }
        }
    }
    else
    {
        QMessageBox msgBox;
        msgBox.setText("Select User to send File");
        msgBox.exec();
    }
}
