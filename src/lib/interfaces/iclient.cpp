#include "iclient.h"

#include "iconnection.h"

#include <QObject>

namespace IPConnect
{

IClient::IClient(QObject* parent) : QObject(parent)
{
}

IClient::~IClient(){}

}
