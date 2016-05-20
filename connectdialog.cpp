#include "connectdialog.h"
#include "ui_connectdialog.h"

ConnectDialog::ConnectDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConnectDialog)
{
    qDebug() << this << "Created Connect Dialog";
    ui->setupUi(this);
    this->setModal(false);
    this->show();
}

ConnectDialog::~ConnectDialog()
{
    qDebug() << this << "Destroyed Connect Dialog";
    delete ui;
}

void ConnectDialog::on_pushButton_clicked()
{
    if(!ui->lineEdit->text().isEmpty())
    {
        this->close();
        qDebug() << this << "IP Address Selected is " << ui->lineEdit->text();
    }
}

void ConnectDialog::on_pushButton_2_clicked()
{
    this->close();
}
