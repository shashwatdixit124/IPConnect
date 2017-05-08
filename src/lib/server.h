#ifndef SERVER_H
#define SERVER_H

#include <QObject>

namespace IPConnect
{
class IServer;

class Server : public IServer
{
public:
	explicit Server(QObject* parent);
	void start() override;

};

}

#endif
 
