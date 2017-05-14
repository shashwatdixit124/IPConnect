import QtQuick 2.0
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0
import QtQuick.Window 2.2

Item {
    id: item
    property bool dark
    property string textColor
    property string bgColor

    signal close()
    signal minimized()
    signal maximized()
    signal drag(real dragX,real dragY)

    Rectangle{
        id: titleBar
        height: 25
        width: parent.width
        color: "transparent"

        Rectangle{
            id: applicationName
            color: "transparent"
            anchors.left: parent.left
            height: parent.height
            anchors.rightMargin: 5

            Text {
                color: item.textColor
                font.pixelSize: 14
                anchors.verticalCenter: parent.verticalCenter
                text: qsTr(" I P C O N N E C T ")
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

        Rectangle{
            id:titleBarToolsMinimize
            height: parent.height
            width: parent.height
            color: "transparent"
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

        Rectangle{
            id:titleBarToolsMaximize
            height: parent.height
            width: parent.height
            color: "transparent"
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

        Rectangle{
            id:titleBarToolsClose
            height: parent.height
            width: parent.height
            color: "transparent"
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

