#ifndef MESSENGER_H
#define MESSENGER_H

#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QList>
#include <QMainWindow>
#include <QMessageBox>
#include <QNetworkInterface>
#include <QTcpSocket>
#include <QTcpServer>
#include <QThread>

#include "client.h"
#include "clientmanager.h"
#include "connectdialog.h"
#include "detaildialog.h"
#include "downloadmanager.h"

namespace Ui {
class Messenger;
}

class Messenger : public QMainWindow
{
    Q_OBJECT

public:
    explicit Messenger(QWidget *parent = 0);
    ~Messenger();

    void connectManually(QString);
    void startServer();

private:
    Ui::Messenger *ui;
    QTcpServer m_server;
    QTcpSocket *m_socket;
    QThread *m_thread;
    QString m_MyUsername;
    QString m_DownloadDirectory;
    QString m_MyIpAddress;
    QMap<QString , Client*> m_users;

    ConnectDialog *m_connectDialog;
    Client *m_client;
    ClientManager *m_manager;
    DetailDialog m_detailDialog;
    DownloadManager m_downloadManger;

signals:
    void accepting(qintptr, Client*, bool);
    void connectToHost(Client* , QString, int);
    void fileAccept();
    void fileReject();
    void sendMessageRequest(QString, Client *);
    void sendFileRequest(QString, Client *);

public slots:
    void defaultChanged(QString,QString);
    void displayWarning(QString,QString);
    void displayQuestion(QString, QString, QString, qint64);
    void handleConnection();
    void handleMessage(QString);
    void removeUser(QString);
    void serverDestroyed();
    void threadFinished();
    void updateList(QString, Client*);

private slots:
    void on_actionConnect_triggered();
    void on_lineEdit_returnPressed();
    void on_actionSend_File_triggered();
};

#endif // MESSENGER_H
