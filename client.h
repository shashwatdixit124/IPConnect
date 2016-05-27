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


    const QTcpSocket *getSocket();

    void setClientUsername(const QString);
    void sendDetail();
    void sendMessage(QString);
    void setSocket(QTcpSocket *t_socket);
    void setUsername(const QString);

    QTcpSocket *m_socket;

protected:
    QString m_ClientUsername;
    QString m_MyUsername;
    bool m_detailAccepted;
    bool m_detailSent;
    QMap<QString, QString> m_request;
    QMap<QString, QString> m_response;

private:
    void handleRequest();
    void processRead(QByteArray);

signals:
    void capturedDetail(QString, Client*);
    void destroyingConnection(QString);
    void gotMessageRequest(QString);
    void warning(QString,QString);
    void question(QString,QString,QString,qint64);

public slots:
    void bytesWritten(qint64 bytes);
    void connected();
    void disconnected();
    void error(QAbstractSocket::SocketError socketError);
    void readyRead();
    void requestSendFile(QString file);
    void stateChanged(QAbstractSocket::SocketState socketState);
};

#endif // CLIENT_H
