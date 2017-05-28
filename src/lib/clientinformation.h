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

#ifndef CLIENTINFORMATION_H
#define CLIENTINFORMATION_H

#include <QObject>

namespace IPConnect
{

class ClientInformation
{

public:
	explicit ClientInformation();
	~ClientInformation();

	qint16 id() const;
	QString name() const;
	QString ip() const;

	void setId(qint16);
	void setName(QString);
	void setIp(QString);

protected:
	qint16 m_id;
	QString m_name;
	QString m_ip;

};

}

Q_DECLARE_METATYPE(IPConnect::ClientInformation);

#endif
