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

#ifndef TRANSFER_H
#define TRANSFER_H

#include "interfaces/itransfer.h"
#include "file.h"
#include "message.h"

#include <QObject>
#include <QIODevice>
#include <QMap>
#include <QTimer>
#include <QThread>

namespace IPConnect
{

class IConnection;

class Transfer : public ITransfer
{
	Q_OBJECT

public:
	explicit Transfer(QObject* parent = nullptr);
	~Transfer();

	void accept() override;
	void reject() override;
	void sendFile() override;

	IConnection* connection();
	void setConnection(IConnection*);

	File file() override;
	void setFile(File);

	int rate() override;
	void setRate(int);

	void setChunkSize(int);
	Q_INVOKABLE void setThread(QThread*);
	void checkForData();
	Q_INVOKABLE void stop();

Q_SIGNALS:
	void requested();
	void destroyTransfer();

protected:
	File m_file;
	IConnection* m_conn;
	int m_rate;
	int m_chunkSize;
	QIODevice* m_source;
	QIODevice* m_destination;
	QTimer m_timer;
	bool m_scheduled;
	bool m_isSender;
	bool m_transfering;
	qint32 m_transfered;
	qint32 m_transferInCycle;
	int m_id;
	int m_progress;
	bool m_stopped;

	void bytesWritten(qint32);
	void readyRead();
	bool checkDevices();
	bool checkTransfer();
	void scheduleTransfer();
	void transfer();

	Message m_request;
	bool m_transferStarted;

	void start();
	void send(Message);
	void handleRead();
	void handleWrite(qint32);
	void processRead(QByteArray);
	void handleRequest();

};

}

#endif
