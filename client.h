#ifndef CLIENT_H
#define CLIENT_H

#include <QDebug>
#include <QFileInfo>
#include <QObject>
#include <QTcpSocket>
#include <QTimer>

#include "filetransfer.h"

class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QObject *parent = 0);
    ~Client();


    const QTcpSocket *getSocket();

    void setClientUsername(const QString);
    void setClientIp(const QString);
    void setDefaultSettings(const QString, const QString, const QString);
    void sendDetail();
    void sendMessage(QString);
    void setSocket(QTcpSocket *);

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
    QString m_DownloadDirectory;
    QString m_MyIp;
    QString m_ClientIp;
    bool m_detailAccepted;
    bool m_detailSent;
    QMap<QString, QString> m_request;
    QMap<QString, QString> m_response;
    QTimer *m_timer;
    bool m_timeractive;

private:
    void setDefaults();
    void sendFile(QString);
    void acceptFile(QString);
    void handleRequest();
    void processRead(QByteArray);
    void write(QString);
    void sendMessageAgain();

signals:
    void capturedDetail(QString, Client*);
    void destroyingConnection(QString);
    void gotMessageRequest(QString);
    void warning(QString,QString);
    void question(QString,QString,QString,qint64);
    void transferFile(QString,QString,QString,qint64,bool);

public slots:
    void bytesWritten(qint64);
    void connected();
    void disconnected();
    void error(QAbstractSocket::SocketError socketError);
    void fileAccepted();
    void fileRejected();
    void fileTransferFinished();
    void readyRead();
    void requestSendFile(QString);
    void stateChanged(QAbstractSocket::SocketState);
    void transferError();
};

#endif // CLIENT_H
