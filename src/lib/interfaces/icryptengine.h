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

#ifndef ICRYPTENGINE_H
#define ICRYPTENGINE_H

#include <QObject>

namespace IPConnect
{

/**
 * Cryptography Engine for encrypting and decrypting data
 * @author Shashwat Dixit <shashwatdixit124@gmail.com>
 */
class ICryptEngine : public QObject
{
	Q_OBJECT

public:
	/**
	 * @brief provides RSA public key
	 * @return pulic key
	 */
	virtual QByteArray publicKey() = 0;

	/**
	 * @brief provides RSA private key
	 * @return private key
	 */
	virtual QByteArray privateKey() = 0;

	/**
	 * @brief generates a random password of length 32
	 * @return password generated
	 */
	virtual QByteArray randomPassword() = 0;

	/**
	 * @brief encrypts data provided with RSA 2048 bit encryption
	 * @param[in] pubKey key to encrypt with
	 * @param[in] input data to be encrypted
	 * @param[out] output encrypted data
	 * @return true if data is encrypted without error, false otherwise
	 */
	virtual bool encryptRSA(QByteArray& pubKey, QByteArray& input, QByteArray& output) = 0;

	/**
	 * @brief decrypts a RSA 2048 bit encrypted data
	 * @param[in] input data to be decrypted
	 * @param[out] output decrypted data
	 * @return
	 */
	virtual bool decryptRSA(QByteArray& input, QByteArray& output) = 0;

	/**
	 * @brief encrypts data provided with AES 256 bit encryption
	 * @param pass passphrase to encrypt with
	 * @param[in] input data to be encrypted
	 * @param[out] output encrypted data
	 * @return true if data is encrypted without error, false otherwise
	 */
	virtual bool encryptAES(QByteArray pass, QByteArray& input, QByteArray& output) = 0;

	/**
	 * @brief decrypts a AES 256 bit encrypted data
	 * @param pass passphrase to decrypt with
	 * @param[in] input data to be decrypted
	 * @param[out] output decrypted data
	 * @return true if data is decrypted without error, false otherwise
	 */
	virtual bool decryptAES(QByteArray pass, QByteArray& input, QByteArray& output) = 0;

protected:
	/**
	 * @brief Constructor
	 */
	explicit ICryptEngine(QObject* parent = nullptr);

	/**
	 * @brief Destructor
	 */
	~ICryptEngine();
};

}

#endif
