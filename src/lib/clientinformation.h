#ifndef CLIENTINFORMATION_H
#define CLIENTINFORMATION_H

#include <QObject>

namespace IPConnect
{

class ClientInformation
{

public:
	explicit ClientInformation();
	~ClientInformation();

	qint16 id();
	QString name();
	QString ip();

	void setId(qint16);
	void setName(QString);
	void setIp(QString);

protected:
	qint16 m_id;
	QString m_name;
	QString m_ip;

private:

};

}

Q_DECLARE_METATYPE(IPConnect::ClientInformation);

#endif
