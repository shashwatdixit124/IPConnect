import QtQuick 2.0
import QtGraphicalEffects 1.0
import api.ui.ipconnect 2.0

Item { id: item
	width: 60
	height: parent.height
	anchors.left: parent.left

	property string theme: IPConnect.theme
	property alias title : applicationNameText.text
	property real tabHeight : 60

	signal messengerClicked
	signal settingsClicked

	FontLoader { id: awesome; source: "qrc:/resources/fontawesome-webfont.ttf" }

	Item { id:sidePane
		anchors.fill: parent

		Column{
			anchors.fill: parent

			Item { id: applicationName
				width: parent.width
				height: item.tabHeight

				Text {
					id: applicationNameText
					color: item.theme
					font.pixelSize: 24
					anchors.centerIn: parent
				}
			}

			Item { id: messenger
				width: parent.width
				height: item.tabHeight

				Text { id: messengerIcon
					anchors.centerIn: parent
					text: qsTr("\uf075")
					color: item.theme
					font.pixelSize: 24
				}

				MouseArea{
					anchors.fill: parent
					cursorShape: Qt.PointingHandCursor
					onClicked: messengerClicked()
				}
			}

			Item { id: settings
				width: parent.width
				height: item.tabHeight

				Text { id: settingsIcon
					anchors.centerIn: parent
					text: qsTr("\uf013")
					color: item.theme
					font.pixelSize: 24
				}

				MouseArea{
					anchors.fill: parent
					cursorShape: Qt.PointingHandCursor
					onClicked: settingsClicked()
				}
			}
		}
	}
}
