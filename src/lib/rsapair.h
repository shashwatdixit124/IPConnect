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

#ifndef RSAPAIR_H
#define RSAPAIR_H

#include <openssl/rsa.h>

#include <QObject>

namespace IPConnect
{

class RSAPair
{
public:
	explicit RSAPair();
	~RSAPair();

	void generateNewPair();
	RSA* publicKey();
	RSA* privateKey();

protected:
	RSA* getPrivateKey(QByteArray&);
	RSA* getPublicKey(QByteArray&);

private:
	RSA* m_pub;
	RSA* m_priv;

};

}
#endif
