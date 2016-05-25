#include "filetransfer.h"

FileTransfer::FileTransfer(QObject *parent) : QObject(parent)
{
    qDebug() << this << "Created";
    m_size = 0;
    m_rate = 0;
}

FileTransfer::~FileTransfer()
{
    qDebug() << this << "Destroyed";
}


int FileTransfer::rate()
{
    return m_rate;
}

void FileTransfer::setRate(int t_value)
{
    m_rate = t_value;
    qDebug() << this << "Rate set to" << t_value;
}

int FileTransfer::size()
{
    return m_size;
}

void FileTransfer::setSize(int t_value)
{
    m_size = t_value;
    qDebug() << this << "Size set to" << t_value;
}

bool FileTransfer::isTransfering()
{
    return m_transfering;
}
