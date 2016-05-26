#include "filetransfer.h"

FileTransfer::FileTransfer(QObject *parent) : QObject(parent)
{
    qDebug() << this << "Created";
    setDefaults();
}

FileTransfer::~FileTransfer()
{
    qDebug() << this << "Destroyed";
}

void FileTransfer::setDefaults()
{
    qDebug() << this << "Setting the defaults";
    m_size = 0;
    m_rate = 0;
    m_transfering = false;
    m_error = "";
    m_source = 0;
    m_destination = 0;
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

QString FileTransfer::errorString()
{
    return m_error;
}

QIODevice *FileTransfer::source()
{
    return m_source;
}

void FileTransfer::setSource(QIODevice *t_device)
{
    m_source = t_device;
    qDebug() << this << "Source set to" << t_device;
    if(m_source->isSequential()) connect(m_source,&QIODevice::readyRead, this, &FileTransfer::readyRead);
}

QIODevice *FileTransfer::destination()
{
    return m_destination;
}

void FileTransfer::setDestination(QIODevice *t_device)
{
    m_destination = t_device;
    qDebug() << this << "Destination set to" << t_device;
    if(m_destination->isSequential()) connect(m_source,&QIODevice::bytesWritten, this, &FileTransfer::bytesWritten);
}

bool FileTransfer::checkDevices()
{
    if(!m_source)
    {
        m_transfering = false;
        m_error = "No source device!";
        qDebug() << this << m_error;
        emit error();
        return false;
    }

    if(!m_destination)
    {
        m_transfering = false;
        m_error = "No destination device!";
        qDebug() << this << m_error;
        emit error();
        return false;
    }

    if(!m_source->isOpen() || !m_source->isReadable())
    {
        m_transfering = false;
        m_error = "Source device is not open or is not readable!";
        qDebug() << this << m_error;
        emit error();
        return false;
    }

    if(!m_destination->isOpen() || !m_destination->isWritable())
    {
        m_transfering = false;
        m_error = "Destination device is not open or is not writable!";
        qDebug() << this << m_error;
        emit error();
        return false;
    }

    return true;
}

bool FileTransfer::checkTransfer()
{
    if(!m_transfering)
    {
        m_error = "Not transfering, aborting!";
        qDebug() << this << m_error;
        emit error();
        return false;
    }

    return true;
}
