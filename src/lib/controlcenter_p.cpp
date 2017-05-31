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

#include "controlcenter_p.h" 

#include "clientmanager.h"
#include "controlcenter.h"
#include "messageserver.h"
#include "transfermanager.h"
#include "usersettings.h"

namespace IPConnect
{

ControlCenterPrivate::ControlCenterPrivate(ControlCenter* parent) : m_cc(parent){}

ControlCenterPrivate::~ControlCenterPrivate(){}

void ControlCenterPrivate::init()
{
	m_userSettings = new UserSettings();
	m_clientManager = new ClientManager(m_cc);
	m_transferManager = new TransferManager(m_cc);
	m_server = new MessageServer(m_cc);
	m_server->start();
}

void ControlCenterPrivate::shutdown()
{
	if(m_server) {
		m_server->shutdown();
		m_server->deleteLater();
	}

	if(m_clientManager) {
		m_clientManager->shutdown();
		m_clientManager->deleteLater();
	}

	if(m_transferManager) {
		m_transferManager->shutdown();
		m_transferManager->deleteLater();
	}

	if(m_userSettings)
		m_userSettings->deleteLater();
}

}
