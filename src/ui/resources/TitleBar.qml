import QtQuick 2.0
import QtQuick.Controls 2.0
import QtQuick.Window 2.2

Item {
	id: item
    property alias title : applicationNameText.text
	property bool dark
	property string textColor
	property string bgColor

	signal close()
	signal minimized()
	signal maximized()
	signal drag(real dragX,real dragY)

	Item{
        id: titleBar
        height:item.height
		width: parent.width

		Item{
			id: applicationName
			anchors.left: parent.left
			height: parent.height
			anchors.rightMargin: 5

			Text {
				id: applicationNameText
				color: item.textColor
				font.pixelSize: 14
				anchors.verticalCenter: parent.verticalCenter
			}
		}

		MouseArea {
			id: titleRegion
			anchors.fill: parent;
			cursorShape: Qt.DragMoveCursor
			property variant clickPos: "1,1"

			onPressed: {
				clickPos  = Qt.point(mouse.x,mouse.y)
			}

			onPositionChanged: item.drag(mouse.x-clickPos.x,mouse.y-clickPos.y)
		}

		Item{
			id:titleBarToolsMinimize
			height: parent.height
			width: parent.height
			anchors.right: titleBarToolsMaximize.left

			Image {
				id: minimizeBtn
                source: "qrc:/resources/min-"+ (item.dark ? "light" : "dark") + ".png"
				anchors.centerIn: parent
			}

			MouseArea{
				id: minimizeRegion
				anchors.fill: minimizeBtn
				onPressed: item.minimized()
			}
		}

		Item{
			id:titleBarToolsMaximize
			height: parent.height
			width: parent.height
			anchors.right: titleBarToolsClose.left

			Image {
				id: maximizeBtn
                source: "qrc:/resources/max-"+ (item.dark ? "light" : "dark") + ".png"
				anchors.centerIn: parent
			}

			MouseArea{
				id: maximizeRegion
				anchors.fill: maximizeBtn
				onPressed: item.maximized()
			}
		}

		Item{
			id:titleBarToolsClose
			height: parent.height
			width: parent.height
			anchors.right: parent.right

			Image {
				id: closeBtn
                source: "qrc:/resources/close-"+ (item.dark ? "light" : "dark") + ".png"
				anchors.centerIn: parent
			}

			MouseArea{
				id:closeRegion
				anchors.fill: closeBtn
				onPressed: item.close()
			}
		}
	}
}

