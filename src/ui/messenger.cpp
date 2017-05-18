#include "messenger.h"

#include <interfaces/iclientmanager.h>

#include <QObject>

namespace IPConnect
{

Messenger::Messenger(IClientManager* cc) : m_cc(cc)
{
}

Messenger::~Messenger()
{
}

void Messenger::sendMessage(int, QString)
{
	
}

}
