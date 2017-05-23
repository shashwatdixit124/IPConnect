import QtQuick 2.0
import QtQuick.Controls 2.0

Item { id:item
	property string theme: "orange"
	property string textColor
	property string bgColor

	Messenger { id: messenger
		theme: item.theme
		anchors.margins: 20
		anchors.fill: parent
		textColor: item.textColor
		bgColor: item.bgColor
	}
}
