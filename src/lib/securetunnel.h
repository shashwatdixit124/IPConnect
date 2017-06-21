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

#ifndef SECURETUNNEL_H
#define SECURETUNNEL_H

#include <QObject>

namespace IPConnect
{
class IConnection;
class Message;

class SecureTunnel : public QObject
{
	Q_OBJECT

public:
	explicit SecureTunnel(QObject* parent = nullptr);
	~SecureTunnel();

	void create();
	bool secure() const;
	void send(QByteArray);
	QByteArray read();
	Message readMessage();
	void setConnection(IConnection*);

Q_SIGNALS:
	void secured();

protected:
	void readyRead();
	void sendKey();
	QByteArray dataSize(quint32);

private:
	bool m_secured;
	bool m_keySent;
	bool m_keyAccepted;
	IConnection* m_conn;
	QByteArray m_clientPublicKey;
};

}


#endif
