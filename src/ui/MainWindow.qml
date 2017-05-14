import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0
import QtQuick.Window 2.2

ApplicationWindow {
    id:root
    visible: true
    minimumWidth: 640
    minimumHeight: 480
    readonly property real xPos: (Screen.width - width)/2
    readonly property real yPos: (Screen.height - height)/2
    x: xPos
    y: yPos
    flags: Qt.FramelessWindowHint
    title: qsTr("IPConnect")

    property bool dark : true
    property string textColor : dark? "#fdfdfd" : "#333"
    property string rootBgColor : dark? "#434347" : "#fdfdfd"
    property string sbBgColor : "#333"
    property string sbTextColor: "#fdfdfd"

    Rectangle{
        anchors.fill: parent
        color: "#333"
        property int borderWidth : 4

        Rectangle{
            id:window
            height: parent.height - parent.borderWidth
            width: parent.width - parent.borderWidth
            anchors.centerIn: parent
            color: root.rootBgColor

            TitleBar{
                id: titleBar
                width: parent.width
                dark: root.dark
                textColor: root.textColor
                bgColor: root.rootBgColor
                anchors.top:parent.top
                onClose: Qt.quit()
                onMaximized: root.showMaximized()
                onMinimized: root.showMinimized()
                onDrag: {
                    root.x += dragX
                    root.y += dragY
                }
            }

            MainView{
                id:mainView
                width: parent.width
                anchors.top: titleBar.bottom
                anchors.bottom: statusBar.top
            }

            StatusBar{
                id:statusBar
                width:parent.width
                height: 16
                anchors.bottom: parent.bottom
                text: qsTr("Server Not Running")
                dark: root.dark
                textColor: root.sbTextColor
                bgColor: root.sbBgColor
                onSizeChanged: {
                    root.width += sizeX
                    root.height += sizeY
                }
            }
        }
    }
}

