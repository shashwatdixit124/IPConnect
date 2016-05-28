#ifndef CLIENT_H
#define CLIENT_H

#include <QDebug>
#include <QFileInfo>
#include <QObject>
#include <QTcpSocket>

#include "filetransfer.h"

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
    QFile *m_file;
    QString m_filename;
    qint64 m_filesize;
    QString m_filepath;
    FileTransfer *m_FileTransfer;
    QString m_ClientUsername;
    bool m_isTransfering;
    QString m_MyUsername;
    bool m_detailAccepted;
    bool m_detailSent;
    QMap<QString, QString> m_request;
    QMap<QString, QString> m_response;

private:
    void setDefaults();
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
    void requestSendFile(QString);
    void stateChanged(QAbstractSocket::SocketState socketState);
};

#endif // CLIENT_H
