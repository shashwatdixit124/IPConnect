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

#ifndef DOWNLOADER_H
#define DOWNLOADER_H

#include "interfaces/idownloader.h"
#include "file.h"

#include <QObject>
#include <QIODevice>
#include <QTimer>

namespace IPConnect
{

class Connection;

class Downloader : public IDownloader
{
	Q_OBJECT

public:
	explicit Downloader(QObject* parent = nullptr);
	~Downloader();

	void start() override;
	void setDefaults();

	File file() override;
	void setFile(File);

	int rate() override;
	void setRate(int);

	void setChunkSize(int);

protected:
	File m_file;
	Connection* m_conn;
	int m_rate;
	int m_chunkSize;
	QIODevice* m_source;
	QIODevice* m_destination;
	QTimer m_timer;
	bool m_isSender;
	bool m_transfering;
	qint32 m_transfered;
	qint32 m_transferInCycle;

	bool checkDevices();

};

}

#endif
