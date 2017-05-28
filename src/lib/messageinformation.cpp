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

#include "messageinformation.h"

#include <QObject>

namespace IPConnect
{

MessageInformation::MessageInformation(){}

MessageInformation::~MessageInformation(){}

qint16 MessageInformation::sent() const
{
	return m_sent;
}

QString MessageInformation::user() const
{
	return m_user;
}

QString MessageInformation::msg() const
{
	return m_msg;
}

void MessageInformation::setSent(bool sent)
{
	m_sent = sent;
}

void MessageInformation::setUser(QString name)
{
	m_user = name;
}

void MessageInformation::setMsg(QString msg)
{
	m_msg = msg;
}

}

