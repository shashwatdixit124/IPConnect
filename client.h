#ifndef CLIENT_H
#define CLIENT_H

#include <QDebug>
#include <QObject>
#include <QTcpSocket>

class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QObject *parent = 0);
    ~Client();

    void setSocket(QTcpSocket *socket);


protected:
    QTcpSocket *m_socket;

signals:

public slots:
    void connected();
    void disconnected();
    void readyRead();
    void bytesWritten(qint64 bytes);
    void stateChanged(QAbstractSocket::SocketState socketState);
    void error(QAbstractSocket::SocketError socketError);
};

#endif // CLIENT_H
