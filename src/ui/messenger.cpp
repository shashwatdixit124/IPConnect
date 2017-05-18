#include "messenger.h"

#include <interfaces/iclientmanager.h>

#include <QObject>

namespace IPConnect
{

Messenger::Messenger(IClientManager* cc) : m_cm(cc)
{
}

Messenger::~Messenger()
{
}

void Messenger::sendMessage(int, QString)
{
	
}

}
