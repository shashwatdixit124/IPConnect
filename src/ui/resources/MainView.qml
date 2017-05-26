import QtQuick 2.5
import QtQuick.Controls 2.1

Item { id:item
	property string textColor
	property string bgColor
	property SideBar sideBar

	Connections{
		target: sideBar
		onMessengerClicked: {
			messenger.visible = true
			appSettings.visible = false
		}
		onSettingsClicked: {
			messenger.visible = false
			appSettings.visible = true
		}
	}

	Messenger { id: messenger
		anchors.margins: 20
		anchors.fill: parent
		textColor: item.textColor
		bgColor: item.bgColor
	}

	AppSettings { id: appSettings
		anchors.margins: 20
		anchors.fill: parent
		textColor: item.textColor
		bgColor: item.bgColor
		visible: false
	}
}
