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
class SecureTunnel;

/**
 * @see ITransfer
 * @author Shashwat Dixit <shashwatdixit124@gmail.com>
 */
class Transfer : public ITransfer
{
	Q_OBJECT

public:
	explicit Transfer(QObject* parent = nullptr);
	~Transfer();

	void accept() override;
	void reject() override;
	void sendFile() override;

	/**
	 * @brief creates a secure tunnel for readin and writing data
	 */
	void createTunnel();

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

protected Q_SLOTS:
	void secured();

protected:
	File m_file;
	IConnection* m_conn;
	SecureTunnel* m_tunnel;
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
	qint32 m_transferInOneSec;
	int m_id;
	int m_progress;
	bool m_stopped;

	/**
	 * @brief called when bytes are written to the connection
	 */
	void bytesWritten(qint32);

	/**
	 * @brief read from connection when new data available
	 */
	void readyRead();

	/**
	 * @brief checks if source and destination are available
	 * @return true if ok, false otherwise
	 */
	bool checkDevices();

	/**
	 * @brief checks if transfer is correct and running
	 * @return true if ok, false otherwise
	 */
	bool checkTransfer();

	/**
	 * @brief schedules a transfer to run
	 * if scheduled data transfered is almost equal to rate, transfer is paused for a second.
	 * to make it send data at m_rate per second
	 */
	void scheduleTransfer();

	/**
	 * @brief transfers the data
	 * it reads a data chunk equal to m_chunkSize bytes from m_source and sends to m_destination
	 */
	void transfer();

	Message m_request;
	bool m_transferStarted;
	bool m_secured;

	/**
	 * @brief starts the transfer
	 */
	void start();

	void send(Message);
	void handleRead();
	void handleWrite(qint32);
	void handleRequest();

};

}

#endif
