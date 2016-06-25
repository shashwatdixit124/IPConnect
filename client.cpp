#include "client.h"

Client::Client(QObject *parent) : QObject(parent)
{
    qDebug() << this << "Client Created";
    setDefaults();
}

Client::~Client()
{
    qDebug() << this << "Client Destroyed";
    emit destroyingConnection(m_ClientUsername);
}

void Client::setDefaults()
{
    m_ClientUsername = "test";
    m_MyUsername = "";
    m_detailSent = false;
    m_detailAccepted = false;
    m_isTransfering = false;
    m_file = 0;
    m_filename = "";
    m_filepath = "";
    m_filesize = 0;
    m_timer = 0;
    m_timeractive = 0;
}

void Client::sendFile(QString t_file)
{
    m_isTransfering = true;

    if(!m_socket) return;
    qDebug() << this << "Sending file " << t_file;

    m_file = new QFile(t_file,this);
    m_FileTransfer = new FileTransfer(this);

    connect(m_FileTransfer,&FileTransfer::error,this, &Client::transferError);
    connect(m_FileTransfer,&FileTransfer::finished,this, &Client::fileTransferFinished);

    if(!m_file->open(QFile::ReadOnly))
    {
        qWarning() << "Could not open file: " << t_file;
        emit warning("Warning","Cannot open File "+t_file);
        return;
    }

    m_FileTransfer->setSource(m_file);
    m_FileTransfer->setDestination(m_socket);
    m_FileTransfer->setRate(1024000);
    m_FileTransfer->setSize(102400);

    QTimer::singleShot(500, m_FileTransfer, SLOT(start()));
}

void Client::acceptFile(QString t_file)
{
    m_isTransfering = true;

    if(!m_socket) return;
    qDebug() << this << "Accepting file " << t_file;

    m_file = new QFile(t_file,this);
    m_FileTransfer = new FileTransfer(this);

    qDebug() << this << "Created: " << m_FileTransfer;

    connect(m_FileTransfer,&FileTransfer::error,this, &Client::transferError);
    connect(m_FileTransfer,&FileTransfer::finished,this, &Client::fileTransferFinished);

    if(!m_file->open(QFile::WriteOnly))
    {
        qWarning() << "Could not open file: " << t_file;
        emit warning("Warning","Cannot open File "+t_file);
        return;
    }

    m_FileTransfer->setSource(m_socket);
    m_FileTransfer->setDestination(m_file);
    m_FileTransfer->setRate(1024000);
    m_FileTransfer->setSize(102400);

    qDebug() << this << "Starting file transfer...";

    m_FileTransfer->start();
}

const QTcpSocket *Client::getSocket()
{
    return m_socket;
}

void Client::setClientUsername(const QString t_username)
{
    m_ClientUsername = t_username;
}

void Client::sendDetail()
{
    qDebug() << this << "Checking for sockets";
    if(m_socket)
    {
        m_detailSent = true;
        QString message = "IPC:CONNECT:REQUEST:"+m_MyUsername;
        m_response.insert("app","IPC");
        m_response.insert("method","CONNECT");
        m_response.insert("option","REQUEST");
        m_response.insert("data",m_MyUsername);
        m_response.insert("message",message);
        write(message);
    }
    else
        qDebug() << this << "socket not available on greetings";
}

void Client::sendMessage(QString t_message)
{
    if(!m_socket) return;
    qDebug() << this << "writing to " << m_socket;
    QString socketMessage = "IPC:MESSAGE:TEXT:"+t_message;
    m_response.insert("message",socketMessage);
    write(socketMessage);
}

void Client::setSocket(QTcpSocket *t_socket)
{
    m_socket = t_socket;
    connect(m_socket,&QTcpSocket::connected, this, &Client::connected,Qt::DirectConnection);
    connect(m_socket,&QTcpSocket::disconnected, this, &Client::disconnected,Qt::DirectConnection);
    connect(m_socket,&QTcpSocket::readyRead, this, &Client::readyRead,Qt::DirectConnection);
    connect(m_socket,&QTcpSocket::bytesWritten, this, &Client::bytesWritten,Qt::DirectConnection);
    connect(m_socket,&QTcpSocket::stateChanged, this, &Client::stateChanged,Qt::DirectConnection);
    connect(m_socket,static_cast<void (QTcpSocket::*)(QAbstractSocket::SocketError)>(&QTcpSocket::error),this,&Client::error,Qt::DirectConnection);
}

void Client::setUsername(const QString t_username)
{
    m_MyUsername = t_username;
}

void Client::processRead(QByteArray t_data)
{
    QString header = t_data;
    qDebug() << this << "Processing GET header = "<<header;
    m_request.insert("request",header);
    qDebug() << this << "request changed to " << m_request.value("request");

    //Header has 4 values APP METHOD OPTION DATA(optional)
    QStringList options = header.split(":");

    if(options.count() >= 1) m_request.insert("app", options.at(0).trimmed());

    if(options.count() >= 2) m_request.insert("method", options.at(1).trimmed());

    if(options.count() >= 3) m_request.insert("option", options.at(2).trimmed());

    if(options.count() >= 4)
    {
        int pos = header.indexOf(":") + m_request.value("method","").size() + m_request.value("option","").size() + 2;
        m_request.insert("data", header.mid(pos + 1));
    }

    handleRequest();
}

void Client::write(QString t_message)
{
    m_socket->write(t_message.toUtf8());
    qDebug() << this << "writing message : " << t_message;
    m_socket->waitForBytesWritten();
    if(!m_timeractive)
    {
        m_timeractive = true;
        qDebug() << this << "***** starting timer";
        m_timer = new QTimer(this);
        connect(m_timer,&QTimer::timeout,this,&Client::sendMessageAgain);
        m_timer->start(500);
    }
}

void Client::sendMessageAgain()
{
    QByteArray msg = m_response.value("message").toUtf8();
    m_socket->write(msg);
}

void Client::handleRequest()
{
    QString method = m_request.value("method","");
    QString option = m_request.value("option","");

    if(method == "CONNECT")
    {
        m_request.insert("message",m_request.value("request"));
        if(option == "REQUEST")
        {
            if(!m_detailAccepted)
            {
                if(m_request.value("option") == "REQUEST")
                {
                    qDebug() << this << "Accepting Detail";
                    QString name = m_request.value("data");
                    setClientUsername(name.trimmed());
                    qDebug() << this << "Accepted request from "<<m_ClientUsername;
                    m_response.insert("app","IPC");
                    m_response.insert("method","CONNECT");
                    m_response.insert("option","REQUEST");
                    m_response.insert("data",m_MyUsername);
                    if(!m_detailSent)
                    {
                        QString message = "IPC:CONNECT:REQUEST:"+m_MyUsername;
                        m_response.insert("message",message);
                        m_detailSent = true;
                        write(message);
                    }
                    m_detailAccepted = true;
                    qDebug() <<this<< "emiting capturedDetail";
                    emit capturedDetail(name.trimmed(),this);
                }
            }
        }
    }
    if(method == "MESSAGE")
    {
        m_request.insert("message",m_request.value("request"));
        m_response.insert("app","IPC");
        m_response.insert("method","MESSAGE");

        if(m_request.value("option") == "TEXT") emit gotMessageRequest(m_ClientUsername+" : "+m_request.value("data"));
    }
    if(method == "FILE")
    {
        m_request.insert("message",m_request.value("request"));
        m_response.insert("app","IPC");
        m_response.insert("method","FILE");
        m_response.insert("data",m_filename);

        if(m_request.value("option") == "RSF")     //IPC:FILE:RSF:65563:AllIsWell.mp4
        {
            QString data = m_request.value("data");
            QStringList options = data.split(":");
            m_filesize = options.at(0).trimmed().toInt();
            int pos = data.indexOf(":");
            m_filename = data.mid(pos + 1);
            emit question("Incomming",m_filename,m_ClientUsername,m_filesize);
        }
        if(m_request.value("option") == "RAF")
        {            
            m_response.insert("option","SFI");
            QString message = "IPC:FILE:SFI:"+m_filename;
            m_response.insert("message",message);
            write(message);
        }
        if(m_request.value("option") == "REJ")
        {
            m_filename = "";
            m_filesize = 0;
            m_filepath = "";
            m_response.insert("app","");
            m_response.insert("method","");
            m_response.insert("data","");

            emit warning("rejected","File "+m_filename+" Rejected ! ");
        }
        if(m_request.value("option") == "SFI")
        {
            qDebug() << "going to accept file";
            acceptFile("/root/Desktop/"+m_filename);
        }
    }
    if(method == "VERIFY")
    {
        if(m_timeractive)
        {
            m_timer->stop();
            m_timeractive = false;
            m_timer->deleteLater();
        }
    }
    return;
}

void Client::connected()
{
    qDebug() << this << "Client Connected";
}

void Client::disconnected()
{
    qDebug() << this << "Client Disconnected";
}

void Client::readyRead()
{
    if(!m_socket) return;
    if(m_isTransfering) return;
    qDebug() << this << "Socket available on ready read " << m_socket->socketDescriptor();
    QString allData = m_socket->readAll();
    if(allData.isEmpty())   return;
    if(allData.contains("IPC:FILE:SFI:"))
        processRead(allData.toUtf8());
    else if(m_request.value("message") == allData)
    {
        qDebug() << this << "***** sending GOT";
        m_socket->write("IPC:VERIFY:GOT:");
    }
    else
        processRead(allData.toUtf8());
}

void Client::requestSendFile(QString t_file)
{
    qDebug() << this << "inside request send file";
    if(!m_socket) return;
    if(m_isTransfering) return;
    qDebug() << this << "writing to " << m_socket;
    qDebug() << this << "selected file is " << t_file;
    m_filepath = t_file;
    QFileInfo fileInfo(t_file);
    m_filename = fileInfo.fileName();
    m_filesize = fileInfo.size();

    m_response.insert("app","IPC");
    m_response.insert("method","FILE");
    m_response.insert("option","RSF");
    m_response.insert("data",QString::number(m_filesize)+":"+m_filename);

    QString socketMessage = "IPC:FILE:RSF:"+QString::number(m_filesize)+":"+m_filename;
    m_response.insert("message",socketMessage);
    write(socketMessage);
}

void Client::bytesWritten(qint64 t_bytes)
{
    Q_UNUSED(t_bytes)
    if(m_isTransfering) return;
    if(!m_socket) return;
    qDebug() << this <<"Socket available on bytesWritten";

    QString method = m_response.value("method");

    if(method == "CONNECT")
    {
        if(!m_detailSent)
        {
            m_detailSent = true;
            qDebug() << this << "detailSent Set to true";
        }
    }

    if(method == "FILE")
    {
        if(m_response.value("option") == "SFI")
        {
            qDebug() << "sending file from bytesWritten";
            sendFile(m_filepath);
        }
    }
}

void Client::stateChanged(QAbstractSocket::SocketState t_socketState)
{
    qDebug() << this << "stateChanged" << m_socket << t_socketState;
}

void Client::transferError()
{
    qDebug() << this <<  "File transfer error: " << m_FileTransfer->errorString();
    m_file->close();
    m_FileTransfer->deleteLater();
}

void Client::error(QAbstractSocket::SocketError socketError)
{
    qDebug() << this << "error" << m_socket << socketError;
}

void Client::fileAccepted()
{
    m_response.insert("app","IPC");
    m_response.insert("method","FILE");
    m_response.insert("option","RAF");
    QString message = "IPC:FILE:RAF:"+m_filename;
    m_response.insert("message",message);
    write(message);
}

void Client::fileRejected()
{
    m_response.insert("app","IPC");
    m_response.insert("method","FILE");
    m_response.insert("option","REJ");
    QString message = "IPC:FILE:REJ:"+m_filename;
    m_response.insert("message",message);
    write(message);
}

void Client::fileTransferFinished()
{
    qDebug() << this << "File transfer finished";
    m_file->close();
    m_FileTransfer->deleteLater();
}
