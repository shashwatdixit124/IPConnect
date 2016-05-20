#include <QApplication>

#include "messenger.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Messenger app;
    app.setWindowTitle("IPConnect");
    app.show();

    return a.exec();
}
