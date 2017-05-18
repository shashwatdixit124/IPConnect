#ifndef MESSAGELIST_H
#define MESSAGELIST_H

#include <messageinformation.h>

#include <QObject>
#include <QAbstractListModel>
#include <QList>

namespace IPConnect
{
class IClientManager;

class MessageList : public QAbstractListModel
{
	Q_OBJECT
public:
	enum UserInfo {
		Sent = Qt::UserRole + 1,
		User,
		Message
	};

	explicit MessageList(IClientManager* cm, QObject* parent = nullptr);
	~MessageList();
	int rowCount(const QModelIndex & parent = QModelIndex()) const;
	QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

public Q_SLOTS:
	void updateList(MessageInformation);

protected:
	QHash<int, QByteArray> roleNames() const;
	IClientManager *m_cm;

private:
	QList<MessageInformation> m_messages;

};

}

#endif

