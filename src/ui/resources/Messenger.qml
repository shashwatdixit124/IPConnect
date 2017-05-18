import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4

Item {
	id:item
	property bool dark
	property string textColor
	property string bgColor
	Item{
		id:messageBox
		height: parent.height
		anchors.left: parent.left
		anchors.right: userListBox.left

		Rectangle{
			id:msgViewBox
			anchors.left: parent.left
			anchors.right: parent.right
			anchors.top: parent.top
			anchors.bottom: msgInputBox.top
			color: item.bgColor
			anchors.margins: 5

			ListView{
				anchors.fill: parent
				id:msgView
				delegate: Text{ text:qsTr("Shashwat: Hi How are You ? ") }
				model: 5
			}
		}

		Rectangle{
			id:msgInputBox
			anchors.left: parent.left
			anchors.right: parent.right
			anchors.bottom: parent.bottom
			height: 40
			anchors.margins: 5
//            color: item.bgColor
			color: "#fdfdfd"


			TextField{
				id:msgInput
				height: parent.height
				anchors.left: parent.left
				anchors.right: sendBtn.left
				font.pixelSize: 14

				style: TextFieldStyle {
					background: Item {}
				}
				onAccepted:{
					if(userList.currentIndex >= 0)
					{
						_messenger.sendMessage(userList.currentItem.id,text);
						text = ""
					}
				}
			}

			Rectangle{
				id: sendBtn
				width: parent.height
				height: parent.height
				anchors.right: parent.right
				color: item.bgColor
				Image{
					id: sendBtnImage
					source: item.dark ? qsTr("qrc:/resources/paper-plane-light-32.png") : qsTr("qrc:/resources/paper-plane-dark-32.png")
					anchors.centerIn: parent
					MouseArea{
						anchors.fill: parent
						onClicked: {
							if(userList.currentIndex >= 0)
							{
								_messenger.sendMessage(userList.currentItem.id,msgInput.text);
								msgInput.text = ""
							}
						}
					}
				}
			}
		}
	}

	Rectangle{
		id:userListBox
		anchors.top: parent.top
		anchors.bottom: parent.bottom
		anchors.right: parent.right
		anchors.margins: 5
		width: 250
		color: item.bgColor


		Component {
			id: userDelegate
			Item {
				id: wrapper
				width: userList.width; height: 40
				Item{
					id:status
					width: parent.height
					height: parent.height
					visible: false

					Rectangle{
						id: statusLight
						height: parent.height/5
						width: parent.width/5
						anchors.centerIn: parent
						color: "green"
						radius: width*0.5
					}
				}

				Text {
					anchors.left: status.right
					anchors.right: parent.right
					anchors.verticalCenter: parent.verticalCenter
					id: user
					text: name
					color: item.textColor
				}

				states: State {
					name: "Current"
					when: wrapper.ListView.isCurrentItem
					PropertyChanges { target: wrapper; x: 5 }
					PropertyChanges { target: status; visible: true }
				}
				transitions: Transition {
					NumberAnimation { properties: "x"; duration: 100 }
				}
				MouseArea {
					anchors.fill: parent
					onClicked: wrapper.ListView.view.currentIndex = index
				}
			}
		}

		Component {
			id: highlightBar
			Rectangle {
				width: userList.width; height: 40
				color: "#333"
				y: userList.currentItem.y;
				Behavior on y { NumberAnimation { duration: 100 } }
			}
		}

		ListView{
			id:userList
			width: parent.width

			model: _users
			delegate: userDelegate

			focus: true
			highlight: highlightBar
			highlightFollowsCurrentItem: false
		}
	}
}
