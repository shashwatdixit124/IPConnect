#include "detaildialog.h"
#include "ui_detaildialog.h"

DetailDialog::DetailDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DetailDialog)
{
    ui->setupUi(this);
    this->setWindowTitle("IPConnect Default Details");
}

DetailDialog::~DetailDialog()
{
    delete ui;
}

void DetailDialog::on_selectBtn_clicked()
{
    dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"), "/home", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    ui->folder->setText(dir);
}

void DetailDialog::on_saveBtn_clicked()
{
    username = ui->lineEdit->text();
    emit saveClicked(dir, username);
    this->close();
}

void DetailDialog::on_cancelBtn_clicked()
{
    this->close();
}
