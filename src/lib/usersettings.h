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

#ifndef USERSETTINGS_H
#define USERSETTINGS_H

#include "interfaces/iusersettings.h"

#include <QObject>
#include <QSettings>

namespace IPConnect
{

/**
 * @see IUserSettings
 * @author Shashwat Dixit <shashwatdixit124@gmail.com>
 */
class UserSettings : public IUserSettings
{
	Q_OBJECT

public:
	explicit UserSettings();
	~UserSettings();

	QString ip() override;
	bool runningFirstTime() override;
	QString name() override;
	QString downloadDir() override;
	QString theme() override;
	void setName(QString) override;
	void setDownloadDir(QString) override;
	void setTheme(QString) override;

protected:
	QString m_name;
	QString m_ip;
	QString m_downloadDir;
	QString m_theme;
	bool m_set;
	bool m_first;
	QSettings m_config;

};

}

#endif
