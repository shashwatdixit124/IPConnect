#ifndef CONNECTION_H
#define CONNECTION_H

#include "interfaces/iconnection.h"

#include <QObject>

namespace IPConnect
{

class Connection : public IConnection
{
	Q_OBJECT

public:
	explicit Connection();
	~Connection();

protected:
	void isError();

	bool isInUse;

};

}

#endif
