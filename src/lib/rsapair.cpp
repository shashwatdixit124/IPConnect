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

#include "rsapair.h"

#include <QDebug>
#include <QFile>
#include <QObject>
#include <QStandardPaths>

namespace IPConnect
{

RSAPair::RSAPair()
{
}

RSAPair::~RSAPair()
{
}

void RSAPair::generateNewPair()
{
	QString tempDir = QStandardPaths::writableLocation(QStandardPaths::TempLocation);
	QString privFileLocation = tempDir+"/ipconnect_priv_key.pem";
	QString pubFileLocation = tempDir+"/ipconnect_pub_key.pem";
	QString genPrivate = "openssl genrsa -out "+privFileLocation+" 2048";
	QString genPublic = "openssl rsa -in "+privFileLocation+" -pubout > "+pubFileLocation;
	system(genPrivate.toStdString().c_str());
	system(genPublic.toStdString().c_str());

	QFile priv(privFileLocation);
	if(!priv.open(QFile::ReadOnly))
	{
		qDebug() << "cannot open file " << privFileLocation ;
		return;
	}

	QByteArray privateKey = priv.readAll();
	priv.close();

	QFile pub(pubFileLocation);
	if(!pub.open(QFile::ReadOnly))
	{
		qDebug() << "cannot open file " << pubFileLocation ;
		return;
	}

	QByteArray publicKey = pub.readAll();
	pub.close();
	
	m_priv = privateKey;
	m_pub = publicKey;
}

QByteArray RSAPair::privateKey()
{
	return m_priv;
}

QByteArray RSAPair::publicKey()
{
	return m_pub;
}

}
