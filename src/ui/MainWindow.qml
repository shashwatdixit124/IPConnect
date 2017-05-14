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

            
        }
    }
}

