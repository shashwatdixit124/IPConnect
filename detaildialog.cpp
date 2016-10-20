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

void DetailDialog::setDefault(QString t_user, QString t_dir)
{
    p_username = t_user;
    p_dir = t_dir;
    ui->lineEdit->setText(p_username);
    ui->folder->setText(p_dir);
}

void DetailDialog::on_selectBtn_clicked()
{
    dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"), "/home", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    ui->folder->setText(dir);
}

void DetailDialog::on_saveBtn_clicked()
{
    username = ui->lineEdit->text();
    if(username.isEmpty())
        username = p_username;
    if(dir.isEmpty())
        dir = p_dir;
    emit saveClicked(dir, username);
    this->close();
}

void DetailDialog::on_cancelBtn_clicked()
{
    this->close();
}
