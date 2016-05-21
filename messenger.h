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

    void startServer();

private:
    Ui::Messenger *ui;
    QTcpServer m_server;
    QTcpSocket *m_socket;
    QThread *m_thread;

    ConnectDialog *m_connectDialog;
    Client *m_client;
    ClientManager *m_manager;

signals:
    void accepting(qintptr handle, Client *client, bool server);

public slots:
    void readyRead();
    void bytesWritten(qint64 bytes);
    void handleConnection();
    void serverDestroyed();
    void threadFinished();

private slots:
    void on_actionConnect_triggered();
};

#endif // MESSENGER_H
