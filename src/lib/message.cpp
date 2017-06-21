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

#include "message.h"

#include <QJsonObject>
#include <QJsonDocument>

namespace IPConnect
{

Message::Message(QByteArray data)
{
	parseJson(data);
}

Message::Message()
{

}

Message Message::fromJson(QByteArray data)
{
	return Message(data);
}

Message::Method Message::method()
{
	return m_method;
}

void Message::setMethod(Method method)
{
	m_method = method;
}

Message::Option Message::option()
{
	return m_option;
}

void Message::setOption(Option option)
{
	m_option = option;
}

QString Message::data(QString key)
{
	return m_data[key].toString();
}

void Message::setData(QString key, QString value)
{
	m_data[key] = value;
}

QByteArray Message::toJson()
{
	QJsonObject root;
	root["METHOD"] = m_method;
	root["OPTION"] = m_option;
	root["DATA"] = m_data;
	QJsonDocument jd(root);
	return jd.toJson();
}

void Message::parseJson(QByteArray data)
{
	QJsonDocument json(QJsonDocument::fromJson(data));
	QJsonObject root = json.object();
	m_method = Method(root["METHOD"].toInt());
	m_option = Option(root["OPTION"].toInt());
	m_data = root["DATA"].toObject();
}

}
