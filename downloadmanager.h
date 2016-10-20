#ifndef DOWNLOADMANAGER_H
#define DOWNLOADMANAGER_H

#include <QDebug>
#include <QMap>
#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QThread>

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
    QMap<QString,QTcpSocket*> m_downloads;
    QTcpServer m_server;
    QTcpSocket* m_socket;

signals:

public slots:
    void handleTransfer();
    void serverDestroyed();
};

#endif // DOWNLOADMANAGER_H
