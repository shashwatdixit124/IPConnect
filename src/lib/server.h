#ifndef SERVER_H
#define SERVER_H

#include "interfaces/iserver.h"

#include <QObject>

namespace IPConnect
{

class Server : public IServer
{
public:
	explicit Server(QObject* parent);
	void start() override;

};

}

#endif
 
