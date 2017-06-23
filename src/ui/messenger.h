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

#ifndef MESSENGER_H
#define MESSENGER_H

#include <QObject>

namespace IPConnect
{
class IClientManager;

/**
 * @brief Messanger to send messages to clients
 */
class Messenger : public QObject
{
	Q_OBJECT
public:
	explicit Messenger(IClientManager *cm);
	~Messenger();

	/**
	 * @brief sends a text message to a client
	 * @param id of client to send message to
	 * @param msg msg to sent to the client
	 */
	void sendMessage(int id, QString msg);

protected:
	IClientManager* m_cm;

};

}

#endif
