#ifndef DOWNLOADMANAGER_H
#define DOWNLOADMANAGER_H

#include <QDebug>
#include <QMap>
#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QThread>

typedef struct {
    QString m_ip;
    QString m_filename;
    QString m_filepath;
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
    Transfer* m_transfer;
    QTcpServer m_server;
    QTcpSocket* m_socket;

signals:

public slots:
    void handleTransfer();
    void readyRead();
    void bytesWritten(qint64);
    void serverDestroyed();
    void transfer(QString,QString,QString,bool);
};

#endif // DOWNLOADMANAGER_H
