/*
 *   This file is part of IPConnect
 *   Copyright 2017 Shashwat Dixit <shashwatdixit124@gmail.com>
 * 
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU Library General Public License as
 *   published by the Free Software Foundation; either version 3 of the
 *   License, or (at your option) any later version.
 * 
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 * 
 *   You should have received a copy of the GNU Library General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include "transfer.h"

#include "interfaces/itransfer.h"
#include "interfaces/iusersettings.h"
#include "interfaces/iconnection.h"
#include "controlcenter.h"
#include "debugtransfer.h"
#include "file.h"
#include "message.h"
#include "securetunnel.h"

#include <QObject>
#include <QFile>
#include <QThread>
#include <QTime>
#include <QDebug>
#include <QHostAddress>

namespace IPConnect
{

Transfer::Transfer(QObject* parent) : ITransfer(parent) , 
	m_rate(5*1024*1024) , m_chunkSize(5*32*1024) , m_source(nullptr) , m_destination(nullptr) , m_progress(0) ,
	m_scheduled(false) , m_isSender(false) , m_transfering(false) , m_id(0) , m_transferStarted(false) , m_stopped(true) ,
	m_tunnel(new SecureTunnel(this))
{
	connect(m_tunnel,&SecureTunnel::secured,this,&Transfer::secured);
}

Transfer::~Transfer()
{
	if(m_source)
	{
		m_source->close();
		m_source->deleteLater();
		m_source = nullptr;
	}
	if(m_destination)
	{
		m_destination->close();
		m_destination->deleteLater();
		m_destination = nullptr;
	}
}

void Transfer::start()
{
	qCDebug(TRANSFER) << this << "Starting the transfer" ;
	if(m_file.action() == File::UNKNOWN){
		qCDebug(TRANSFER) << this << "( ERROR ) File Not Set";
		emit error();
		return;
	}

	if(m_file.action() == File::SEND){
		QString filepath = m_file.path() + "/" + m_file.name();
		m_source = new QFile(filepath);
		if(!m_source->open(QFile::ReadOnly))
		{
			qWarning() << "Could not open file: " << filepath;
			emit error();
			return;
		}
		m_destination = m_conn;
		connect(m_destination,&QIODevice::bytesWritten,this,&Transfer::bytesWritten);
		m_isSender = true;
	}

	if(m_file.action() == File::RECIEVE){
		m_source = m_conn;
		connect(m_source,&QIODevice::readyRead,this,&Transfer::readyRead);
		QString filepath = m_file.path() + "/" + m_file.name();
		m_destination = new QFile(filepath);
		if(!m_destination->open(QFile::WriteOnly))
		{
			qWarning() << "Could not open file: " << filepath;
			emit error();
			return;
		}
		m_isSender = false;
	}

	m_transferStarted = true;
	m_stopped = false;
	m_transfering = true;
	m_transfered = 0;

	checkDevices();

	if(m_isSender && m_source->bytesAvailable() > 0)
	{
		qCDebug(TRANSFER) << this <<"transfer started";
		scheduleTransfer();
	}
}

void Transfer::stop()
{
	qCDebug(TRANSFER) << this << "Stopping the transfer";
	m_timer.stop();
	m_stopped = true;
	m_transfering = false;

	emit destroyTransfer();
}

void Transfer::secured()
{
	qCDebug(TRANSFER) << this << "transfer is secured";
	if(m_file.action() == File::SEND)
		sendFile();
	else
		checkForData();
}

IConnection* Transfer::connection()
{
	return m_conn;
}

void Transfer::setConnection(IConnection* conn)
{
	if(!conn)
		return;

	m_conn = conn;
	m_tunnel->setConnection(conn);
	connect(m_conn,&IConnection::dataAvailable,this,&Transfer::handleRead);
	connect(m_conn,&IConnection::errorOccurred,this,&Transfer::stop);
}

void Transfer::sendFile()
{
	if(m_file.action() != File::SEND)
		return;

	qCDebug(TRANSFER) << this << "sending file " << m_file.name() ;

	QString username = ControlCenter::instance()->userSettings()->name();
	Message m;
	m.setMethod(Message::TRANSFER);
	m.setOption(Message::RSF);
	m.setData("USERNAME",username);
	m.setData("FILENAME",m_file.name());
	m.setData("FILESIZE",QString::number(m_file.size()));
	send(m);
}

void Transfer::send(Message m)
{
	if(!m_tunnel->secure())
		return;

	if(m_conn)
	{
		m_tunnel->send(m.toJson());
		m_conn->flush();
		m_conn->waitForBytesWritten();
	}
	else
		qCDebug(TRANSFER) << "connection not available on write" ;
}

File Transfer::file()
{
	return m_file;
}

void Transfer::setFile(File file)
{
	m_file = file;
}

int Transfer::rate()
{
	return m_rate;
}

void Transfer::setRate(int rate)
{
	m_rate = rate;
}

void Transfer::createTunnel()
{
	m_tunnel->create();
}

void Transfer::setChunkSize(int cs)
{
	m_chunkSize = cs;
}

void Transfer::setThread(QThread* thr)
{
	if(m_conn)
		m_conn->moveToThread(thr);
	moveToThread(thr);
}

void Transfer::checkForData()
{
	qCDebug(TRANSFER) << "checking for data" ;
	if(m_conn && m_conn->hasUnreadData()) {
		m_request = m_tunnel->readMessage();
		handleRequest();
	}
	else 
		qCDebug(TRANSFER) << "no data found" ;
}

void Transfer::bytesWritten(qint32)
{
	scheduleTransfer();
}

void Transfer::readyRead()
{
	scheduleTransfer();
}

bool Transfer::checkDevices()
{
	if(!m_source)
	{
		m_transfering = false;
		qCDebug(TRANSFERRIGOR) << this << "No source device!";
		emit error();
		return false;
	}

	if(!m_destination)
	{
		m_transfering = false;
		qCDebug(TRANSFERRIGOR) << this << "No destination device!";
		emit error();
		return false;
	}

	if(!m_source->isOpen() || !m_source->isReadable())
	{
		m_transfering = false;
		qCDebug(TRANSFERRIGOR) << this << "Source device is not open or is not readable!";
		emit error();
		return false;
	}

	if(!m_destination->isOpen() || !m_destination->isWritable())
	{
		m_transfering = false;
		qCDebug(TRANSFERRIGOR) << this << "Destination device is not open or is not writable!";
		emit error();
		return false;
	}

	return true;
}

bool Transfer::checkTransfer()
{
	if(!m_transfering)
	{
		qCDebug(TRANSFERRIGOR) << this << "Not transfering, aborting!";
		emit error();
		return false;
	}
	if(m_transferInCycle >= m_rate)
	{
		qCDebug(TRANSFERRIGOR) << this << "Rate exeeded, not allowed to transfer!";
		emit error();
		return false;
	}

	return true;
}

void Transfer::scheduleTransfer()
{
	if(!m_tunnel->secure())
		return;

	if(m_stopped)
		return;

	qCDebug(TRANSFERRIGOR) << this << "scheduleTransfer called";

	if(m_scheduled)
	{
		qCDebug(TRANSFERRIGOR) << this << "Exiting scheduleTransfer due to: waiting on timer";
		return;
	}

	if(!m_transfering)
	{
		qCDebug(TRANSFERRIGOR) << this << "Exiting scheduleTransfer due to: not transfering";
		return;
	}

	if(m_isSender && m_source->bytesAvailable() <= 0)
	{
		qCDebug(TRANSFERRIGOR) << this << "Exiting scheduleTransfer due to: no bytes available to be read";
		return;
	}

	if(!m_isSender && !m_conn->hasUnreadData())
	{
		qCDebug(TRANSFERRIGOR) << this << "Exiting scheduleTransfer due to: no bytes available to be read";
		return;
	}

	int prediction = m_transferInCycle + m_chunkSize;
	if(prediction <= m_rate)
	{
		qCDebug(TRANSFERRIGOR) << this << "calling transfer from scheduleTransfer";
		transfer();
	}
	else
	{
		int current = QTime::currentTime().msec();
		int delay = 1000 - current;
		qCDebug(TRANSFERRIGOR) << this << "Rate limit (" << m_rate << ") exeeded in prediction (" << m_transferInCycle << " to " <<  prediction << "), delaying transfer for " << delay << "ms";
		m_transferInCycle = 0;
		m_scheduled = true;
		m_timer.singleShot(delay,this,&Transfer::transfer);
	}
}

void Transfer::transfer()
{
	if(!m_tunnel->secure())
		return;

	if(m_stopped)
		return;

	m_scheduled = false;
	m_transfering = true;

	if(!checkDevices()) 
		return;

	if(!checkTransfer()) 
		return;

	QByteArray buffer;
	if(m_isSender)
		buffer = m_source->read(m_chunkSize);
	else
		buffer = m_tunnel->read();

	qCDebug(TRANSFER) << this << "writting to destination: " << buffer.length() ;

	quint64 len = buffer.length();
	if(m_isSender)
		m_tunnel->send(buffer);
	else
		m_destination->write(buffer);
	m_transferInCycle += len;
	m_transfered += len;
	if(!m_isSender) {
		int temp = ((double)m_transfered/m_file.size())*100;
		if(m_progress != temp) {
			m_progress = temp;
			m_file.setProgress(m_progress);
			m_file.setTransfered(m_transfered);
			int rate = len/(1024*1024);
			m_file.setRate(rate);
			emit progress(m_progress,m_transfered,rate);
		}
	}

	if(!m_isSender && m_file.size() == m_transfered)
	{
		qCDebug(TRANSFER) << this << "Stopping due to recieved whole file";
		stop();
	}
	else if(!m_isSender)
	{
		qCDebug(TRANSFERRIGOR) << this << "Still Recieving total recieved = " << m_transfered ;
	}

	if(m_isSender && m_source->bytesAvailable() == 0)
	{
		qCDebug(TRANSFER) << this << "Stopping due to end of file";
		return;
	}
	else if(m_isSender)
	{
		qCDebug(TRANSFERRIGOR) << this << "Still Sending total sent = " << m_transfered ;
	}

	if(m_transfering == false)
		return;

	if(!m_source->isSequential() && m_source->bytesAvailable() > 0)
	{
		qCDebug(TRANSFERRIGOR) << this << "Source still has bytes, scheduling a transfer";
		scheduleTransfer();
	}
}

void Transfer::handleRead()
{
	if(!m_tunnel->secure())
		return;

	if(m_transferStarted)
		return;

	m_request = m_tunnel->readMessage();
	handleRequest();
}

void Transfer::handleWrite(qint32)
{
	if(!m_tunnel->secure())
		return;

	if(m_transferStarted)
		return;
}

void Transfer::accept()
{
	Message m;
	m.setMethod(Message::TRANSFER);
	m.setOption(Message::RAF);
	m.setData("FILENAME",m_file.name());
	send(m);
	disconnect(m_conn,&IConnection::readyRead,this,&Transfer::handleRead);
	connect(m_conn,&IConnection::readyRead,this,&Transfer::readyRead);
	start();
}

void Transfer::reject()
{
	Message m;
	m.setMethod(Message::TRANSFER);
	m.setOption(Message::REJ);
	m.setData("FILENAME",m_file.name());
	send(m);
	stop();
}

void Transfer::processRead(QByteArray data)
{
	m_request = Message::fromJson(data);
	handleRequest();
}

void Transfer::handleRequest()
{
	if(m_request.method() == Message::TRANSFER)
	{
		if(m_request.option() == Message::RSF && m_file.action() == File::UNKNOWN)
		{
			QString clientName = m_request.data("USERNAME");
			QString filesize = m_request.data("FILESIZE");
			QString filename = m_request.data("FILENAME");

			qCDebug(TRANSFER) << this << "recieved rsf request" ;

			if(clientName.isEmpty() || filesize.isEmpty() || filename.isEmpty())
				return;

			m_file.setName(filename);
			m_file.setSize(filesize.toULongLong());
			m_file.setUserName(clientName);
			m_file.setUrl(m_conn->peerAddress().toString());
			m_file.setPath(ControlCenter::instance()->userSettings()->downloadDir());
			m_file.setAction(File::RECIEVE);

			qCDebug(TRANSFER) << this << "requested transfer" ;
			emit requested();
		}
		if(m_request.option() == Message::RAF && m_file.action() == File::SEND)
		{
			disconnect(m_conn,&IConnection::readyRead,this,&Transfer::handleRead);
			connect(m_conn,&IConnection::readyRead,this,&Transfer::readyRead);
			start();
		}
		if(m_request.option() == Message::REJ && m_file.action() == File::SEND)
		{
			stop();
		}
	}
	return;
}

}
