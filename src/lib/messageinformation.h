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

#ifndef MESSAGEINFORMATION_H
#define MESSAGEINFORMATION_H

#include <QObject>

namespace IPConnect
{

class MessageInformation
{

public:
	explicit MessageInformation();
	~MessageInformation();

	qint16 sent() const;
	QString user() const;
	QString msg() const;

	void setSent(bool);
	void setUser(QString);
	void setMsg(QString);

protected:
	bool m_sent;
	QString m_user;
	QString m_msg;

};

}

Q_DECLARE_METATYPE(IPConnect::MessageInformation);

#endif
