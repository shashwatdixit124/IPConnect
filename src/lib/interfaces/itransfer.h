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

#ifndef IDOWNLOADER_H
#define IDOWNLOADER_H

#include <QObject>

namespace IPConnect
{

class File;

/**
 * Interface for a Transfer
 * @author Shashwat Dixit <shashwatdixit124@gmail.com>
 */
class ITransfer : public QObject
{
	Q_OBJECT

public:
	/**
	 * @brief accepts the transfer
	 */
	virtual void accept() = 0;

	/**
	 * @brief rejects the transfer
	 */
	virtual void reject() = 0;

	/**
	 * @brief sends the detail of file to be sent (i.e. initiates the transfer @c request)
	 */
	virtual void sendFile() = 0;

	/**
	 * @brief provides the information about File to be sent or recieved
	 * @return File
	 */
	virtual File file() = 0;

	/**
	 * @brief provide the rate at which transfer will be done
	 * @return rate in bytes
	 */
	virtual int rate() = 0;

Q_SIGNALS:
	/**
	 * @brief emitted if an error occured while transfering
	 */
	void error();

	/**
	 * @brief emitted every sec to provide the progress information
	 * @param progress transfer completed in percentage
	 * @param transfered total file transfered
	 * @param rate transfer rate at which recieving data
	 */
	void progress(int progress,quint64 transfered,int rate);

protected:
	/**
	 * @brief Constructor
	 */
	explicit ITransfer(QObject* parent = nullptr);

	/**
	 * @brief Destructor
	 */
	~ITransfer();

};

}

#endif
