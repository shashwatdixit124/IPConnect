#include "messenger.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Messenger w;
    w.setWindowTitle("IPConnect");
    w.show();

    return a.exec();
}
