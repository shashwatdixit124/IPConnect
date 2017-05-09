#include "controlcenter.h"

#include <iostream>
#include <QApplication>

using namespace IPConnect;

int main(int argc, char* argv[])
{
	QApplication a(argc,argv);
	ControlCenter::init();
	a.exec();

	std::cout << "Destroying Control Center" ;
	ControlCenter::instance()->shutdown();
	return 0;
}
