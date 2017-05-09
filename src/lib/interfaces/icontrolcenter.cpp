#include "icontrolcenter.h"

#include <QObject>

namespace IPConnect
{

IControlCenter* IControlCenter::m_instance = nullptr;

IControlCenter::IControlCenter(QObject *parent) : QObject(parent)
{
	Q_ASSERT(m_instance == nullptr);
}

IControlCenter::~IControlCenter(){}

IControlCenter* IControlCenter::instance()
{
	return m_instance;
}

}
