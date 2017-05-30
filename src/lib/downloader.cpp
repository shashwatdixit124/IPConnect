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

#include "downloader.h"

#include "interfaces/idownloader.h"
#include "connection.h"
#include "debugdownload.h"
#include "file.h"

#include <QObject>
#include <QFile>

namespace IPConnect
{

Downloader::Downloader(QObject* parent) : IDownloader(parent) , 
	m_rate(5*1024*1024) , m_chunkSize(5*32*1024) , m_source(nullptr) ,
	m_destination(nullptr) , m_isSender(false) , m_transfering(false)
{
}

Downloader::~Downloader()
{
}

void Downloader::start()
{
	if(m_file.action() == File::UNKNOWN){
		qCDebug(DOWNLOAD) << this << "( ERROR ) File Not Set";
		return;
	}

	if(m_file.action() == File::SEND){
		m_source = new QFile(m_file.path() + "/" + m_file.name());
		m_destination = m_conn;
		m_isSender = true;
	}

	if(m_file.action() == File::RECIEVE){
		m_source = m_conn;
		m_destination = new QFile(m_file.path() + "/" + m_file.name());
		m_isSender = false;
	}

}

File Downloader::file()
{
	return m_file;
}

void Downloader::setFile(File file)
{
	m_file = file;
}

int Downloader::rate()
{
	return m_rate;
}

void Downloader::setRate(int rate)
{
	m_rate = rate;
}

void Downloader::setChunkSize(int cs)
{
	m_chunkSize = cs;
}

bool Downloader::checkDevices()
{
    if(!m_source)
    {
        m_transfering = false;
        qCDebug(DOWNLOAD) << this << "No source device!";
        return false;
    }

    if(!m_destination)
    {
        m_transfering = false;
        qCDebug(DOWNLOAD) << this << "No destination device!";
        return false;
    }

    if(!m_source->isOpen() || !m_source->isReadable())
    {
        m_transfering = false;
        qCDebug(DOWNLOAD) << this << "Source device is not open or is not readable!";
        return false;
    }

    if(!m_destination->isOpen() || !m_destination->isWritable())
    {
        m_transfering = false;
        qCDebug(DOWNLOAD) << this << "Destination device is not open or is not writable!";
        return false;
    }

    return true;
}

}
