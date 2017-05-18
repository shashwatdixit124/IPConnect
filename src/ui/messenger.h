#ifndef MESSENGER_H
#define MESSENGER_H

#include <QObject>

namespace IPConnect
{
class IClientManager;

class Messenger : public QObject
{
	Q_OBJECT
public:
	explicit Messenger(IClientManager *cm);
	~Messenger();

	Q_INVOKABLE void sendMessage(int,QString);

protected:
	IClientManager* m_cm;

};

}

#endif
