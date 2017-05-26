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
	a.setApplicationVersion("2.0");

	ControlCenter::init();
	QObject::connect(&a,&QApplication::aboutToQuit,ControlCenter::instance(),&ControlCenter::shutdown);
	IPConnectWindow w;
	w.show();

	a.exec();
	ControlCenter::quit();
	return 0;
}
