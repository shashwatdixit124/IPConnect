#ifndef FILETRANSFER_H
#define FILETRANSFER_H

#include <QObject>
#include <QDebug>
#include <QIODevice>
#include <QTimer>

class FileTransfer : public QObject
{
    Q_OBJECT
public:
    explicit FileTransfer(QObject *parent = 0);
    ~FileTransfer();

    void setDefaults();
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
    void error();

public slots:
    void start();
    void stop();

protected:
    int m_rate;
    int m_size;
    bool m_transfering;
    qint64 m_transfered;
    QString m_error;
    QIODevice *m_source;
    QIODevice *m_destination;
    QTimer m_timer;
    bool m_scheduled;

    bool checkDevices();
    bool checkTransfer();

protected slots:
    void bytesWritten(qint64);
    void readyRead();
    void transfer();
};

#endif // FILETRANSFER_H
