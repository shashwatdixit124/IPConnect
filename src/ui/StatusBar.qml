import QtQuick 2.0
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0
import QtQuick.Window 2.2

Item {
    id:item
    property alias text : statusText.text
    property bool dark
    property string textColor
    property string bgColor

    signal sizeChanged(real sizeX, real sizeY)

    Rectangle{
        id:statusBarWrapper
        anchors.fill: parent
        color: "transparent"

        Rectangle{
            id:status
            height: parent.height
            anchors.left: parent.left
            anchors.right: resizeRect.left
            color: item.bgColor

            Text {
                id: statusText
                color: item.textColor
                anchors.verticalCenter: parent.verticalCenter
                text: item.text
            }

        }

        Rectangle{
            id:resizeRect
            width: parent.height
            height: parent.height
            anchors.right: parent.right
            color: item.bgColor

            MouseArea{
                id:resizeRegion
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

