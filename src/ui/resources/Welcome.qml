import QtQuick 2.5
import QtQuick.Controls 2.1
import QtQuick.Dialogs 1.2
import QtQml 2.2

Popup { id: setting

	property string theme: "#16a085"

	modal: true
	height: parent.height
	width: parent.width
	closePolicy: Popup.NoAutoClose

	Component.onCompleted: {
		open()
		welcomeTimer.start()
	}

	contentItem: Rectangle{

		Timer { id: welcomeTimer
			running: false
			interval: 1000
			repeat: false
			onTriggered: {
				welcomeText.opacity = 0
				settingForm.visible = true
			}
		}

		Text { id: welcomeText
			text: qsTr("Welcome To IPConnect")
			color: root.theme
			font.pixelSize: 30
			anchors.centerIn: parent
			Behavior on opacity { NumberAnimation { duration: 500 } }
		}

		Item { id: settingForm
			anchors.fill: parent
			visible: false

			Behavior on visible { NumberAnimation { duration: 600 } }

			Text { id: settingsTitle
				height: 40
				anchors.top: parent.top
				anchors.horizontalCenter: parent.horizontalCenter
				font.pixelSize: 20
				color: root.theme
				text: qsTr("Configure Settings")
			}
		}
	}
}
