#ifndef FILETRANSFER_H
#define FILETRANSFER_H

#include <QObject>
#include <QDebug>
#include <QIODevice>

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
    QString errorString();
    QIODevice *source();
    void setSource(QIODevice *device);
    QIODevice *destination();
    void setDestination(QIODevice *device);

signals:

public slots:

protected:
    int m_rate;
    int m_size;
    bool m_transfering;
    QString m_error;
    QIODevice *m_source;
    QIODevice *m_destination;
};

#endif // FILETRANSFER_H
