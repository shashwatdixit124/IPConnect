import QtQuick 2.5
import QtQuick.Controls 2.1
import api.ui.ipconnect 2.0

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
		onQuickConnectClicked: {
			quickConnect.open()
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
		onUpdated:{
			IPConnect.userName = username
			IPConnect.downloadDir = ddir
			IPConnect.theme = themeColor
		}
	}

	QuickConnect{ id: quickConnect }
}
