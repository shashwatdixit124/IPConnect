import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0
import QtQuick.Window 2.2

ApplicationWindow {
	id:root
	visible: true
	minimumWidth: 640
	minimumHeight: 480
	flags: Qt.FramelessWindowHint

	property bool dark : true
	property string textColor : dark? "#fdfdfd" : "#333"
	property string rootBgColor : dark? "#434347" : "#fdfdfd"
	property string sbBgColor : "#333"
	property string sbTextColor: "#fdfdfd"

	Rectangle{
		id:window
		anchors.fill:parent
		border.width: 1.5
		border.color: "#333"
		color: root.rootBgColor

		TitleBar{
			id: titleBar
			title: qsTr(" I P C O N N E C T ")
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
			height: 20
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

