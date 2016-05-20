#ifndef MESSENGER_H
#define MESSENGER_H

#include <QMainWindow>
#include <QTcpServer>
#include <QTcpSocket>

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
};

#endif // MESSENGER_H
