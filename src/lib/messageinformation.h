#ifndef MESSAGEINFORMATION_H
#define MESSAGEINFORMATION_H

#include <QObject>

namespace IPConnect
{

class MessageInformation
{

public:
	explicit MessageInformation();
	~MessageInformation();

	qint16 sent() const;
	QString user() const;
	QString msg() const;

	void setSent(bool);
	void setUser(QString);
	void setMsg(QString);

protected:
	bool m_sent;
	QString m_user;
	QString m_msg;

};

}

Q_DECLARE_METATYPE(IPConnect::MessageInformation);

#endif
