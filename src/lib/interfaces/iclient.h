#ifndef ICLIENT_H
#define ICLIENT_H

#include <QObject>

namespace IPConnect
{

class IConnection;

class IClient : public QObject
{
	Q_OBJECT

public:
	virtual IConnection* getConnection() = 0;
	virtual QString name() = 0;
	virtual QString ip() = 0;

protected:
	IClient(QObject* parent = nullptr);
	~IClient();

};

}

#endif
