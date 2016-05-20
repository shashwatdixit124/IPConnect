#include "messenger.h"
#include "ui_messenger.h"

Messenger::Messenger(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Messenger)
{
    ui->setupUi(this);
    startServer();
}

Messenger::~Messenger()
{
    delete ui;
}

void Messenger::startServer()
{
    if(this->m_server.listen(QHostAddress::Any, 2424))
    {
        qDebug() << this << "Server started on thread ";
    }
    else
    {
        qDebug() << this << "Server Could not start";
    }
}
