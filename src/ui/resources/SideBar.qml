import QtQuick 2.0

Item { id: root
	width: 60
	height: parent.height
	anchors.left: parent.left

	property string theme: "orange"
	property alias title : applicationNameText.text
	property real tabHeight : 40
	property string color : "orange"

	FontLoader { id: awesome; source: "qrc:/resources/fontawesome-webfont.ttf" }

	Item { id:sidePane
		anchors.fill: parent
		Item { id: applicationName
			width: parent.width
			height: width

			Text { id: applicationNameText
				color: root.theme
				font.pixelSize: 24
				anchors.centerIn: parent
			}
		}
	}
}
