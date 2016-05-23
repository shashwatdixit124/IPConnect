#ifndef MESSENGER_H
#define MESSENGER_H

#include <QDebug>
#include <QList>
#include <QMainWindow>
#include <QTcpSocket>
#include <QTcpServer>
#include <QThread>

#include "client.h"
#include "clientmanager.h"
#include "connectdialog.h"

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
    QMap<QString , Client*> m_users;

    ConnectDialog *m_connectDialog;
    Client *m_client;
    ClientManager *m_manager;

signals:
    void accepting(qintptr, Client*, bool);
    void connectToHost(Client* , QString, int);
    void sendMessageRequest(QString, Client *);

public slots:
    void bytesWritten(qint64);
    void handleConnection();
    void handleMessage(QString);
    void readyRead();
    void removeUser(QString);
    void serverDestroyed();
    void threadFinished();
    void updateList(QString, Client*);

private slots:
    void on_actionConnect_triggered();
    void on_lineEdit_returnPressed();
};

#endif // MESSENGER_H
