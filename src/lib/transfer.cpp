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
#include "connection.h"
#include "debugtransfer.h"
#include "file.h"

#include <QObject>
#include <QFile>
#include <QTime>

namespace IPConnect
{

Transfer::Transfer(QObject* parent) : ITransfer(parent) , 
	m_rate(5*1024*1024) , m_chunkSize(5*32*1024) , m_source(nullptr) , m_destination(nullptr) ,
	m_scheduled(false) , m_isSender(false) , m_transfering(false)
{
}

Transfer::~Transfer()
{
	if(m_source)
	{
		m_source->deleteLater();
		m_source = nullptr;
	}
	if(m_destination)
	{
		m_destination->deleteLater();
		m_destination = nullptr;
	}
}

void Transfer::start()
{
	if(m_file.action() == File::UNKNOWN){
		qCDebug(TRANSFER) << this << "( ERROR ) File Not Set";
		emit error();
		return;
	}

	if(m_file.action() == File::SEND){
		m_source = new QFile(m_file.path() + "/" + m_file.name());
		m_destination = m_conn;
		connect(m_destination,&QIODevice::bytesWritten,this,&Transfer::bytesWritten);
		m_isSender = true;
	}

	if(m_file.action() == File::RECIEVE){
		m_source = m_conn;
		connect(m_source,&QIODevice::readyRead,this,&Transfer::readyRead);
		m_destination = new QFile(m_file.path() + "/" + m_file.name());
		m_isSender = false;
	}

}

void Transfer::stop()
{
	qDebug() << this << "Stopping the transfer";
	m_timer.stop();
	m_transfering = false;
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

void Transfer::setChunkSize(int cs)
{
	m_chunkSize = cs;
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
		qCDebug(TRANSFER) << this << "No source device!";
		emit error();
		return false;
	}

	if(!m_destination)
	{
		m_transfering = false;
		qCDebug(TRANSFER) << this << "No destination device!";
		emit error();
		return false;
	}

	if(!m_source->isOpen() || !m_source->isReadable())
	{
		m_transfering = false;
		qCDebug(TRANSFER) << this << "Source device is not open or is not readable!";
		emit error();
		return false;
	}

	if(!m_destination->isOpen() || !m_destination->isWritable())
	{
		m_transfering = false;
		qCDebug(TRANSFER) << this << "Destination device is not open or is not writable!";
		emit error();
		return false;
	}

	return true;
}

bool Transfer::checkTransfer()
{
	if(!m_transfering)
	{
		qCDebug(TRANSFER) << this << "Not transfering, aborting!";
		emit error();
		return false;
	}
	if(m_transfered >= m_rate)
	{
		qCDebug(TRANSFER) << this << "Rate exeeded, not allowed to transfer!";
		emit error();
		return false;
	}

	return true;
}

void Transfer::scheduleTransfer()
{
	qCDebug(TRANSFER) << this << "scheduleTransfer called";

	if(m_scheduled)
	{
		qCDebug(TRANSFER) << this << "Exiting scheduleTransfer due to: waiting on timer";
		return;
	}

	if(!m_transfering)
	{
		qCDebug(TRANSFER) << this << "Exiting scheduleTransfer due to: not transfering";
		return;
	}

	if(m_source->bytesAvailable() <= 0)
	{
		qCDebug(TRANSFER) << this << "Exiting scheduleTransfer due to: no bytes available to be read";
		return;
	}

	int prediction = m_transferInCycle + m_chunkSize;
	if(prediction <= m_rate)
	{
		qCDebug(TRANSFER) << this << "calling transfer from scheduleTransfer";
		transfer();
	}
	else
	{
		int current = QTime::currentTime().msec();
		int delay = 1000 - current;
		qCDebug(TRANSFER) << this << "Rate limit (" << m_rate << ") exeeded in prediction (" << m_transfered << " to " <<  prediction << "), delaying transfer for " << delay << "ms";
		m_transfered = 0;
		m_scheduled = true;
		m_timer.singleShot(delay,this,&Transfer::transfer);
	}
}

void Transfer::transfer()
{
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
		buffer = m_source->read(m_source->bytesAvailable());

	qCDebug(TRANSFER) << this << "writting to destination: " << buffer.length();

	m_destination->write(buffer);
	m_transferInCycle += buffer.length();
	m_transfered += buffer.length();

	if(!m_isSender)
	{
		if(m_file.size() == m_transfered)
		{
			emit finished();
			stop();
		}
	}

	if(!m_source->isSequential() && m_source->bytesAvailable() == 0)
	{
		qCDebug(TRANSFER) << this << "Stopping due to end of file";
		stop();
	}

	if(m_transfering == false)
		return;

	if(!m_source->isSequential() && m_source->bytesAvailable() > 0)
	{
		qCDebug(TRANSFER) << this << "Source still has bytes, scheduling a transfer";
		scheduleTransfer();
	}
}

}
