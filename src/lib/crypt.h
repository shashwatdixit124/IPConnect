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

#include <openssl/rsa.h>

#include <QObject>

#define PADDING RSA_PKCS1_PADDING
#define KEYSIZE 32
#define IVSIZE 32
#define BLOCKSIZE 256
#define SALTSIZE 8

namespace IPConnect
{

/**
 * @brief Implements methods to encrypt and decrypt data with RSA and AES using OpenSSl library
 *
 * @author Shashwat Dixit <shashwatdixit124@gmail.com>
 */
class Crypt : public QObject
{
	Q_OBJECT

public:
	explicit Crypt(QObject* parent = nullptr);
	~Crypt();

	RSA* getPublicKey(QByteArray &);
	RSA* getPrivateKey(QByteArray &);
	void freeKey(RSA*);
	QByteArray randomBytes(int);

	QByteArray encryptRSA(RSA *key, QByteArray &data);
	QByteArray decryptRSA(RSA *key, QByteArray &data);
	QByteArray encryptAES(QByteArray, QByteArray&);
	QByteArray decryptAES(QByteArray, QByteArray&);
};

}
