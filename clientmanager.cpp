#include "clientmanager.h"

ClientManager::ClientManager(QObject *parent) : QObject(parent)
{
    qDebug() << this << "Client Manager created on thread" << QThread::currentThread();
}

ClientManager::~ClientManager()
{
    qDebug() << this << "Client Manager Destroyed";
}
