import QtQuick 2.0
import QtQuick.Controls 2.0
import QtQuick.Window 2.2
import api.ui.ipconnect 2.0

Item { id:item
	property string theme: IPConnect.theme
	property alias text : statusText.text
	property string textColor
	property string bgColor

	signal sizeChanged(real sizeX, real sizeY)

	Item { id:statusBarWrapper
		anchors.fill: parent

		Item { id:status
			height: parent.height
			anchors.left: parent.left
			anchors.right: resizeRect.left

			Text { id: statusText
				color: item.theme
				x: 10
				anchors.verticalCenter: parent.verticalCenter
				text: item.text
			}
		}

		Image { id:resizeRect
			source: "qrc:/resources/triangle-32.png"
			width: 12
			height: 12
			anchors.right: parent.right
			anchors.bottom: parent.bottom

			MouseArea { id:resizeRegion
				anchors.fill: parent
				cursorShape: Qt.SizeFDiagCursor
				property variant clickPos: "1,1"
				onPressed: {
					clickPos  = Qt.point(mouse.x,mouse.y)
				}
				onPositionChanged: item.sizeChanged(mouse.x-clickPos.x,mouse.y-clickPos.y)
			}
		}
	}
}

