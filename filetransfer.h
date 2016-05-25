#ifndef FILETRANSFER_H
#define FILETRANSFER_H

#include <QObject>
#include <QDebug>

class FileTransfer : public QObject
{
    Q_OBJECT
public:
    explicit FileTransfer(QObject *parent = 0);
    ~FileTransfer();

    int rate();
    void setRate(int);
    int size();
    void setSize(int);
    bool isTransfering();

signals:

public slots:

protected:
    int m_rate;
    int m_size;
    bool m_transfering;
};

#endif // FILETRANSFER_H
