import QtQuick 2.5
import QtQuick.Controls 2.1
import api.ui.ipconnect 2.0

Popup { id: item
	height: 200
	width: 400
	x: (parent.width - 60 - width)/2
	y: (parent.height - height)/2
	modal: true

	FontLoader { id: awesome; source: "qrc:/resources/fontawesome-webfont.ttf" }

	contentItem: Item{
		anchors.fill: parent
		Text { id: quickConnectText
			text: qsTr("Quick Connect")
			font.pixelSize: 24
			anchors.top: parent.top
			anchors.topMargin: 10
			anchors.horizontalCenter: parent.horizontalCenter
		}

		Item { id: quickConnectForm
			anchors.centerIn: parent
			height: 40
			width: quickConnectUrl.width + quickConnectBtn.width

			Row {
				anchors.fill: parent
				TextField { id: quickConnectUrl
					height: parent.height
					width: 300
					placeholderText: qsTr(" URL")
				}
				Button { id: quickConnectBtn
					height: parent.height
					width: height
					text: qsTr("\uf0c1")
					onClicked:{
						IPConnect.quickConnect(quickConnectUrl.text)
						quickConnectUrl.text = ""
						close()
					}
				}
			}
		}
	}
}
