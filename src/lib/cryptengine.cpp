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

#include "cryptengine.h"

#include "interfaces/icryptengine.h"
#include "crypt.h"
#include "rsapair.h"

#include <openssl/rsa.h>

#include <QObject>

namespace IPConnect
{

CryptEngine::CryptEngine(QObject* parent) : ICryptEngine(parent) , m_crypt(new Crypt(this))
{
	RSAPair rsa;
	rsa.generateNewPair();
	m_privKey = rsa.privateKey();
	m_pubKey = rsa.publicKey();
	m_privRSA = m_crypt->getPrivateKey(m_privKey);
	m_pubRSA = m_crypt->getPublicKey(m_pubKey);
}

CryptEngine::~CryptEngine()
{
	if(m_privRSA)
		m_crypt->freeKey(m_privRSA);
	if(m_pubRSA)
		m_crypt->freeKey(m_pubRSA);
	m_crypt->deleteLater();
}

QByteArray CryptEngine::publicKey()
{
	return m_pubKey;
}

QByteArray CryptEngine::privateKey()
{
	return m_privKey;
}

bool CryptEngine::encryptData(QByteArray& pubKey , QByteArray& input , QByteArray& output)
{
	RSA* pub = m_crypt->getPublicKey(pubKey);
	output = m_crypt->encryptRSA(pub,input);
	m_crypt->freeKey(pub);
	if(output.isEmpty())
		return false;
	else
		return true;
}

bool CryptEngine::decryptData(QByteArray& input , QByteArray& output)
{
	output = m_crypt->decryptRSA(m_privRSA,input);
	if(output.isEmpty())
		return false;
	else
		return true;
	return false;
}

}
