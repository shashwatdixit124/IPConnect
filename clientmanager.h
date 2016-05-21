#ifndef CLIENTMANAGER_H
#define CLIENTMANAGER_H

#include <QDebug>
#include <QObject>
#include <QReadWriteLock>
#include <QTcpSocket>
#include <QThread>

#include "client.h"

class ClientManager : public QObject
{
    Q_OBJECT
public:
    explicit ClientManager(QObject *parent = 0);
    ~ClientManager();

protected slots:

signals:

public slots:
};

#endif // CLIENTMANAGER_H
