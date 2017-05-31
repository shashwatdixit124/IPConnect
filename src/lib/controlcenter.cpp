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

#include "controlcenter.h"

#include "interfaces/iclientmanager.h"
#include "interfaces/icontrolcenter.h"
#include "interfaces/iserver.h"
#include "interfaces/itransfermanager.h"
#include "interfaces/iusersettings.h"
#include "clientmanager.h"
#include "controlcenter_p.h"
#include "messageserver.h"
#include "transfermanager.h"
#include "transferserver.h"
#include "usersettings.h"

#include <QObject>

namespace IPConnect
{

ControlCenter* ControlCenter::m_instance = nullptr;

ControlCenter::ControlCenter(QObject* parent) : IControlCenter(parent) , m_ccp(new ControlCenterPrivate(this)){}

ControlCenter::~ControlCenter()
{
	shutdown();
}

ControlCenter* ControlCenter::instance()
{
	return m_instance;
}

void ControlCenter::init()
{
	m_instance = new ControlCenter();
	m_instance->m_ccp->init();
}

void ControlCenter::quit()
{
	m_instance->deleteLater();
	m_instance = nullptr;
}

IServer* ControlCenter::messageServer()
{
	return m_ccp->m_messageServer;
}

IServer* ControlCenter::transferServer()
{
	return m_ccp->m_transferServer;
}

IClientManager* ControlCenter::clientManager()
{
	return m_ccp->m_clientManager;
}

ITransferManager* ControlCenter::transferManager()
{
	return m_ccp->m_transferManager;
}

IUserSettings* ControlCenter::userSettings()
{
	return m_ccp->m_userSettings;
}

void ControlCenter::shutdown()
{
	m_instance->m_ccp->shutdown();
	delete m_ccp;
}

}
