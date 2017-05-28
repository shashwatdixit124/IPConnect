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

#include "controlcenter.h"
#include "ipconnectwindow.h"

#include <iostream>
#include <QApplication>
#include <QIcon>

using namespace IPConnect;

int main(int argc, char* argv[])
{
	QApplication a(argc,argv);
	a.setApplicationName("IPConnect");
	a.setOrganizationName("IPConnect");
	a.setOrganizationDomain("https://github.com/shashwatdixit124/IPConnect");
	a.setApplicationVersion("2.0");

	ControlCenter::init();
	QObject::connect(&a,&QApplication::aboutToQuit,ControlCenter::instance(),&ControlCenter::shutdown);
	IPConnectWindow w;
	w.show();

	a.exec();
	ControlCenter::quit();
	return 0;
}
