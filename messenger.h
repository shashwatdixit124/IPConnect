#ifndef MESSENGER_H
#define MESSENGER_H

#include <QDebug>
#include <QMainWindow>
#include <QTcpSocket>
#include <QTcpServer>
#include <QThread>

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

public slots:
    void readyRead();
    void bytesWritten(qint64 bytes);
    void handleConnection();
    void serverDestroyed();

};

#endif // MESSENGER_H
