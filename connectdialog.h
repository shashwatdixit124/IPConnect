#ifndef CONNECTDIALOG_H
#define CONNECTDIALOG_H

#include <QDebug>
#include <QDialog>

namespace Ui {
class ConnectDialog;
}

class ConnectDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConnectDialog(QWidget *parent = 0);
    ~ConnectDialog();

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();

signals:
    void connect(QString);

private:
    Ui::ConnectDialog *ui;
};

#endif // CONNECTDIALOG_H
