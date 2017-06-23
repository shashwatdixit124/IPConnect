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

#ifndef ICONTROLCENTER_H
#define ICONTROLCENTER_H

#include <QObject>

namespace IPConnect
{

class IServer;
class IClientManager;
class ICryptEngine;
class ITransferManager;
class IUserSettings;
/**
 * Allow access to instances of
 * @li @c MessageServer
 * @li @c TransferServer
 * @li @c ClientManager
 * @li @c CryptEngine
 * @li @c TransferManager
 * @li @c UserSettings
 * @author Shashwat Dixit <shashwatdixit124@gmail.com>
 */
class IControlCenter : public QObject
{
	Q_OBJECT

public:
	/**
	 * @brief static instance of ControlCenter
	 * @return IControlCenter
	 */
	static IControlCenter* instance();

	/**
	 * @brief access to instance of MessageServer
	 * @return IServer
	 */
	virtual IServer* messageServer() = 0;

	/**
	 * @brief access to instance of TransferServer
	 * @return IServer
	 */
	virtual IServer* transferServer() = 0;

	/**
	 * @brief access to instance of ClientManager
	 * @return IClientManager
	 */
	virtual IClientManager* clientManager() = 0;

	/**
	 * @brief access to instance of CryptEngine
	 * @return ICryptEngine
	 */
	virtual ICryptEngine* cryptEngine() = 0;

	/**
	 * @brief access to instance of Transfermanager
	 * @return ITransferManager
	 */
	virtual ITransferManager* transferManager() = 0;

	/**
	 * @brief access to instance of UserSettings
	 * @return IUserSettings
	 */
	virtual IUserSettings* userSettings() = 0;

protected:
	/**
	 * @brief static instance of IControlCenter for using from anywhere in the application
	 */
	static IControlCenter* m_instance;

	/**
	 * @brief Constructor
	 */
	explicit IControlCenter(QObject* parent = nullptr);

	/**
	 * @brief Destructor
	 */
	~IControlCenter();

};

}

#endif
