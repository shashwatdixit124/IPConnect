#include "controlcenter.h"
#include "ipconnectwindow.h"

#include <iostream>
#include <QApplication>

using namespace IPConnect;

int main(int argc, char* argv[])
{
	QApplication a(argc,argv);
	ControlCenter::init();
	IPConnectWindow w;
	w.show();
	a.exec();

	std::cout << "Destroying Control Center" ;
	ControlCenter::instance()->shutdown();
	return 0;
}
