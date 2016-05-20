#ifndef MESSENGER_H
#define MESSENGER_H

#include <QMainWindow>

namespace Ui {
class Messenger;
}

class Messenger : public QMainWindow
{
    Q_OBJECT

public:
    explicit Messenger(QWidget *parent = 0);
    ~Messenger();

private:
    Ui::Messenger *ui;
};

#endif // MESSENGER_H
