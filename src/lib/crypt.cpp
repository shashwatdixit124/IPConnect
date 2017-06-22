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
#include <openssl/err.h>
#include <openssl/aes.h>
#include <openssl/rand.h>

#include <QDebug>
#include <QObject>

namespace IPConnect
{

Crypt::Crypt(QObject* parent) : QObject(parent)
{
	ERR_load_CRYPTO_strings();
	OpenSSL_add_all_algorithms();
    OPENSSL_config(NULL);
}

Crypt::~Crypt()
{
	EVP_cleanup();
	ERR_free_strings();
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

QByteArray Crypt::encryptAES(QByteArray passphrase, QByteArray &data)
{
	QByteArray msalt = randomBytes(SALTSIZE);
	int rounds = 1;
	unsigned char key[KEYSIZE];
	unsigned char iv[IVSIZE];

	const unsigned char* salt = (const unsigned char*) msalt.constData();
	const unsigned char* password = (const unsigned char*) passphrase.constData();

	int i = EVP_BytesToKey(EVP_aes_256_cbc(), EVP_sha1(), salt,password, passphrase.length(),rounds,key,iv);

	if(i != KEYSIZE)
	{
		qCritical() << "EVP_BytesToKey() error: " <<  ERR_error_string(ERR_get_error(), NULL);
		return QByteArray();
	}

	EVP_CIPHER_CTX en;
	EVP_CIPHER_CTX_init(&en);

	if(!EVP_EncryptInit_ex(&en, EVP_aes_256_cbc(),NULL,key, iv))
	{
		qCritical() << "EVP_EncryptInit_ex() failed " <<  ERR_error_string(ERR_get_error(), NULL);
		return QByteArray();
	}

	char *input = data.data();
	int len = data.size();

	int c_len = len + AES_BLOCK_SIZE, f_len = 0;
	unsigned char *ciphertext = (unsigned char*)malloc(c_len);

	if(!EVP_EncryptInit_ex(&en, NULL, NULL, NULL, NULL))
	{
		qCritical() << "EVP_EncryptInit_ex() failed " <<  ERR_error_string(ERR_get_error(), NULL) ;
		return QByteArray();
	}

	if(!EVP_EncryptUpdate(&en, ciphertext, &c_len,(unsigned char *)input, len))
	{
		qCritical() << "EVP_EncryptUpdate() failed " <<  ERR_error_string(ERR_get_error(), NULL) ;
		return QByteArray();
	}

	if(!EVP_EncryptFinal(&en, ciphertext+c_len, &f_len))
	{
		qCritical() << "EVP_EncryptFinal_ex() failed " <<  ERR_error_string(ERR_get_error(), NULL) ;
		return QByteArray();
	}

	len = c_len + f_len;
	EVP_CIPHER_CTX_cipher(&en);

	//ciphertext

	QByteArray encrypted = QByteArray(reinterpret_cast<char*>(ciphertext), len);
	QByteArray finished;
	finished.append("Salted__");
	finished.append(msalt);
	finished.append(encrypted);

	free(ciphertext);

	return finished;
}

QByteArray Crypt::decryptAES(QByteArray passphrase, QByteArray &data)
{
	QByteArray msalt;
	if(QString(data.mid(0,8)) == "Salted__")
	{
		msalt = data.mid(8,8);
		data = data.mid(16);
	}
	else
	{
		qWarning() << "Could not load salt from data!";
		msalt = randomBytes(SALTSIZE);
	}

	int rounds = 1;
	unsigned char key[KEYSIZE];
	unsigned char iv[IVSIZE];
	const unsigned char* salt = (const unsigned char*)msalt.constData();
	const unsigned char* password = (const unsigned char*)passphrase.data();

	int i = EVP_BytesToKey(EVP_aes_256_cbc(), EVP_sha1(), salt,password, passphrase.length(),rounds,key,iv);

	if(i != KEYSIZE)
	{
		qCritical() << "EVP_BytesToKey() error: " << ERR_error_string(ERR_get_error(), NULL);
		return QByteArray();
	}

	EVP_CIPHER_CTX de;
	EVP_CIPHER_CTX_init(&de);

	if(!EVP_DecryptInit_ex(&de,EVP_aes_256_cbc(), NULL, key,iv ))
	{
		qCritical() << "EVP_DecryptInit_ex() failed" << ERR_error_string(ERR_get_error(), NULL);
		return QByteArray();
	}

	char *input = data.data();
	int len = data.size();

	int p_len = len, f_len = 0;
	unsigned char *plaintext = (unsigned char *)malloc(p_len + AES_BLOCK_SIZE);

	//May have to do this multiple times for large data???
	if(!EVP_DecryptUpdate(&de, plaintext, &p_len, (unsigned char *)input, len))
	{
		qCritical() << "EVP_DecryptUpdate() failed " <<  ERR_error_string(ERR_get_error(), NULL);
		return QByteArray();
	}

	if(!EVP_DecryptFinal_ex(&de,plaintext+p_len,&f_len))
	{
		qCritical() << "EVP_DecryptFinal_ex() failed " <<  ERR_error_string(ERR_get_error(), NULL);
		return QByteArray();
	}

	len = p_len + f_len;

	EVP_CIPHER_CTX_cleanup(&de);


	QByteArray decrypted = QByteArray(reinterpret_cast<char*>(plaintext), len);
	free(plaintext);

	return decrypted;

}

QByteArray Crypt::randomBytes(int size)
{
	unsigned char arr[size];
	RAND_bytes(arr,size);

	QByteArray buffer = QByteArray(reinterpret_cast<char*>(arr), size);
	return buffer;
}

}
