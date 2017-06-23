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

/**
 * @brief A Secure Tunnel for Connection
 *
 * exchanges RSA public keys and AES passphrases for creating secure tunnel
 *
 * @author Shashwat Dixit <shashwatdixit124@gmail.com>
 */
class SecureTunnel : public QObject
{
	Q_OBJECT

public:
	/**
	 * @brief CTOR
	 */
	explicit SecureTunnel(QObject* parent = nullptr);

	/**
	 * @brief DTOR
	 */
	~SecureTunnel();

	/**
	 * @brief starts enchanging keys
	 */
	void create();

	/**
	 * @brief to check if tunnel is secured or not
	 * @return true if tunnel is secured, fasle otherwise
	 */
	bool secure() const;

	/**
	 * @brief send the data by encrypting it
	 * @param data to send
	 */
	void send(QByteArray data);

	/**
	 * @brief reads all the available data and decrypts it
	 * @return Decrypted data
	 */
	QByteArray read();

	/**
	 * @brief reads a single message from all available data on connection
	 * @return Message read
	 */
	Message readMessage();

	/**
	 * @brief sets the connection to write or read from
	 * @param conn Connection to Secure
	 */
	void setConnection(IConnection* conn);

Q_SIGNALS:
	/**
	 * @brief emitted when the connection is secure
	 */
	void secured();

protected:
	/**
	 * @brief to read from connection when new data is available
	 */
	void readyRead();

	/**
	 * @brief sends RSA public key to the client
	 */
	void sendKey();

	/**
	 * @brief sends AES Passphrase
	 * first encrypts it with RSA 2048 bit with client's public key
	 */
	void sendAESPass();

	/**
	 * @brief recieves AES Passphrase
	 * recieves it in 2048 bit encrypted format
	 */
	void getAESPass();

	/**
	 * @brief recieves RSA public key from the client
	 */
	void getRSAPub();

	/**
	 * @brief converts an integer size to a 8 character string format
	 * @return converted text
	 */
	QByteArray dataSize(quint32 size);

	/**
	 * @brief reads a single data from the connection
	 * @return
	 */
	QByteArray readRawData();

	/**
	 * @brief reads a single AES 256 bit encrypted data from the connection and decrypts it
	 * @return decrypted data
	 */
	QByteArray readAESDecryptedData();

	/**
	 * @brief reads a single RSA 2048 bit encrypted data from the connection and decrypts it
	 * @return decrypted data
	 */
	QByteArray readRSADecryptedData();

private:
	bool m_secured;
	bool m_gotRSAPub;
	bool m_keySent;
	bool m_keyAccepted;
	IConnection* m_conn;
	QByteArray m_clientPublicKey;
	QString m_aesPassPhrase;
};

}


#endif
