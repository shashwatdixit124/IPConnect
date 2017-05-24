import QtQuick 2.5
import QtQuick.Controls 2.1
import QtQuick.Dialogs 1.2

Item { id:item
	property string theme: "orange"
	property string textColor
	property string bgColor

	Messenger { id: messenger
		theme: item.theme
		anchors.margins: 20
		anchors.fill: parent
		textColor: item.textColor
		bgColor: item.bgColor
	}

	Popup { id: setting
		modal: true
		height: 300
		width: 400
		x: (parent.width - width)/2
		y: (parent.height - height)/2
		closePolicy: Popup.NoAutoClose
		Component.onCompleted: open()
		contentItem: Rectangle{
			Text { id: settingsTitle
				height: 40
				anchors.top: parent.top
				anchors.horizontalCenter: parent.horizontalCenter
				font.pixelSize: 20
				text: qsTr("Settings")
			}

			SwipeView { id: inputs

				clip: true
				anchors.top: settingsTitle.bottom
				anchors.bottom: btngrp1.top
				width: parent.width
				interactive: false

				Page{
					TextField { id: userName
						anchors.centerIn: parent
						placeholderText: qsTr("User Name")
					}
				}

				Page{
					Rectangle { id: downloadDir
						anchors.centerIn: parent
						width: parent.width - 20
						height: 40
						TextField { id: downloadDirVal
							anchors.left: parent.left
							anchors.right: selectBtn.left
							height: parent.height
							text: dirDialog.folder
						}
						Button { id: selectBtn
							anchors.right: parent.right
							text: qsTr("Select")
							height: parent.height
							onClicked: dirDialog.open()
						}
					}
					FileDialog { id: dirDialog
						selectExisting: true
						selectFolder: true
					}
				}

				Page{
					ColorDialog { id: colorDialog
						currentColor: "#fd5151"
						color: currentColor
					}
					Rectangle { id: colorTheme
						anchors.centerIn: parent
						width: 140
						height: 40
						Text { id: colorText
							anchors.left: parent.left
							anchors.right: colorView.left
							anchors.verticalCenter: parent.verticalCenter
							anchors.margins: 10
							width: 100
							text: colorDialog.color
						}
						Rectangle { id: colorView
							anchors.right: parent.right
							color: colorDialog.color
							width: parent.height
							height: parent.height
							MouseArea{
								anchors.fill: parent
								onClicked: colorDialog.open()
							}
						}
					}
				}
			}

			Item { id: btngrp1
				width: parent.width
				height: 50
				anchors.bottom: parent.bottom

				Button { id: prevBtn
					anchors.left: parent.left
					height: parent.height
					text: qsTr("Prev")
					background: Rectangle{
						implicitWidth: 100
						color: "#f0f0f0"
					}

					onClicked: {
						if(inputs.currentIndex > 0)
							inputs.setCurrentIndex(inputs.currentIndex - 1)
					}
				}

				Button { id: nextBtn
					property bool disable: (page == 0 && userName.text == "") ||
											(page == 1 && downloadDirVal.text == "") ||
											(page == 2 && colorText.text == "") ? true : false
					property alias page: inputs.currentIndex
					anchors.right: parent.right
					height: parent.height
					text: page == 2 ? qsTr("Finish") : qsTr("Next")

					background: Rectangle{
						implicitWidth: 100
						color: nextBtn.disable ? "#d0d0d0" : "#f0f0f0"
					}

					onClicked: {
						if(!disable)
						{
							if(page < 2)
								inputs.setCurrentIndex(page + 1)
							else if(page == 2)
							{
								//TODO save settings
								setting.close()
							}
						}
					}
				}
			}
		}
	}
}
