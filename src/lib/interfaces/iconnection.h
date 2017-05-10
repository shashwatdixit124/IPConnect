#ifndef ICONNECTION_H
#define ICONNECTION_H

#include <QTcpSocket>

namespace IPConnect
{

class IConnection : public QTcpSocket
{
	Q_OBJECT

protected:
	explicit IConnection();
	~IConnection();

};

}

#endif
