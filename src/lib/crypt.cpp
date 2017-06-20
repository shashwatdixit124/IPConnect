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

#include "crypt.h"

#include <openssl/rsa.h>
#include <openssl/engine.h>
#include <openssl/pem.h>
#include <openssl/conf.h>
#include <openssl/bio.h>
#include <openssl/evp.h>

#include <QDebug>
#include <QObject>

namespace IPConnect
{

Crypt::Crypt(QObject* parent) : QObject(parent)
{
	OpenSSL_add_all_algorithms();
    OPENSSL_config(NULL);
}

Crypt::~Crypt()
{
	EVP_cleanup();
}

RSA* Crypt::getPrivateKey(QByteArray& data)
{
	const char* privateKeyStr = data.constData();
	BIO* bio = BIO_new_mem_buf((void*)privateKeyStr, -1);
	BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL);

	RSA* rsaPrivKey = PEM_read_bio_RSAPrivateKey(bio,NULL, NULL, NULL);
	if(!rsaPrivKey)
	{
		qCritical() << "Could not load private key" ;
	}

	BIO_free(bio);
	return rsaPrivKey;
}

RSA* Crypt::getPublicKey(QByteArray &data)
{
	const char* publicKeyStr = data.constData();
	BIO* bio = BIO_new_mem_buf((void*)publicKeyStr, -1);
	BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL);

	RSA* rsaPubKey = PEM_read_bio_RSA_PUBKEY(bio,NULL, NULL, NULL);
	if(!rsaPubKey)
	{
		qCritical() << "Could not load public key" ;
	}

	BIO_free(bio);
	return rsaPubKey;
}

void Crypt::freeKey(RSA* key)
{
	RSA_free(key);
}

QByteArray Crypt::encryptRSA(RSA *key, QByteArray &data)
{
    QByteArray result;
    int dataSize = data.length();
    const unsigned char* rawData = (const unsigned char*)data.constData();
    int rsaLen = RSA_size(key);

    unsigned char* encryptedData = (unsigned char*)malloc(rsaLen);
    int edlen = RSA_public_encrypt(dataSize, rawData, encryptedData, key, PADDING);

    if(edlen == -1)
    {
        qCritical() << this << "Could not encrypt: " ;
        return result;
    }

    result = QByteArray(reinterpret_cast<char*>(encryptedData), edlen);
    free(encryptedData);

    return result;
}

QByteArray Crypt::decryptRSA(RSA *key, QByteArray &data)
{
    QByteArray result;
    const unsigned char* encryptedData = (const unsigned char*)data.constData();

    int rsaLen = RSA_size(key);

    unsigned char* decryptedData = (unsigned char*)malloc(rsaLen);
    int ddlen = RSA_private_decrypt(rsaLen, encryptedData, decryptedData, key, PADDING);

    if(ddlen == -1)
    {
        qCritical() << this << "Could not decrypt: " ;
        return result;
    }

    result = QByteArray(reinterpret_cast<char*>(decryptedData), ddlen);
    free(decryptedData);

    return result;
}

}
