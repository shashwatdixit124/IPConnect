#ifndef DOWNLOADMANAGER_H
#define DOWNLOADMANAGER_H

#include <QDebug>
#include <QFile>
#include <QMap>
#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QThread>

#include "filetransfer.h"

typedef struct {
    QString m_ip;
    QString m_filename;
    QString m_filepath;
    qint64 m_filesize;
    bool isSending;
} Transfer;

class DownloadManager : public QObject
{
    Q_OBJECT
public:
    explicit DownloadManager(QObject *parent = 0);
    ~DownloadManager();

private:
    QString m_MyUsername;
    QString m_DownloadDirectory;
    QMap<QTcpSocket*,QThread*> m_threads;
    QMap<QString,Transfer*> m_transfers;
    QMap<QString,QString>m_request;
    Transfer* m_transfer;
    QTcpServer m_server;
    QTcpSocket* m_socket;

protected:
    void processRead(QString, QTcpSocket*);
    void sendFile(Transfer *,QTcpSocket*);
    void acceptFile(Transfer *, QTcpSocket*);

signals:

public slots:
    void handleTransfer();
    void readyRead();
    void bytesWritten(qint64);
    void serverDestroyed();
    void transfer(QString,QString,QString,qint64,bool);
    void fileTransferFinished();
};

#endif // DOWNLOADMANAGER_H
