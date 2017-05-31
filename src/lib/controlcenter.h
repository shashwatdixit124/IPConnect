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

#ifndef CONTROLCENTER_H
#define CONTROLCENTER_H

#include "interfaces/icontrolcenter.h"

#include <QObject>

namespace IPConnect
{

class IServer;
class IClientManager;
class ITransferManager;
class ControlCenterPrivate;
class IUserSettings;

class ControlCenter : public IControlCenter
{
	Q_OBJECT
public:
	explicit ControlCenter(QObject* parent = nullptr);
	~ControlCenter();
	static ControlCenter* instance();
	static void init();
	static void quit();

	IServer* messageServer() override;
	IClientManager* clientManager() override;
	ITransferManager* transferManager() override;
	IUserSettings* userSettings() override;

public Q_SLOTS:
	void shutdown();

protected:
	friend ControlCenterPrivate;
	ControlCenterPrivate* m_ccp;
	static ControlCenter* m_instance;

};

}

#endif
