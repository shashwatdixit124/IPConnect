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

class IControlCenter : public QObject
{
	Q_OBJECT

public:
	static IControlCenter* instance();
	virtual IServer* messageServer() = 0;
	virtual IServer* transferServer() = 0;
	virtual IClientManager* clientManager() = 0;
	virtual ICryptEngine* cryptEngine() = 0;
	virtual ITransferManager* transferManager() = 0;
	virtual IUserSettings* userSettings() = 0;

protected:
	static IControlCenter* m_instance;
	explicit IControlCenter(QObject* parent = nullptr);
	~IControlCenter();

};

}

#endif
