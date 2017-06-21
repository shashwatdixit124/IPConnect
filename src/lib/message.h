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

#ifndef MESSAGE_H
#define MESSAGE_H

#include <QObject>
#include <QJsonObject>

class Message
{
public:
	Message(QByteArray);
	Message();

	enum Method{
		SECURE,
		CONNECT,
		TRANSFER,
		MSG
	};

	enum Option{
		RSA,
		REQUEST,
		TEXT,
		RSF,
		RAF,
		REJ
	};

	static Message fromJson(QByteArray);

	Method method();
	void setMethod(Method);

	Option option();
	void setOption(Option);

	QString data(QString);
	void setData(QString, QString);

	QByteArray toJson();

protected:
	Method m_method;
	Option m_option;
	QJsonObject m_data;

	void parseJson(QByteArray);
};

#endif
