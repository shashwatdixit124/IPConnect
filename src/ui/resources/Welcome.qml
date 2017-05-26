import QtQuick 2.5
import QtQuick.Controls 2.1
import QtQuick.Dialogs 1.2
import QtQml 2.2
import api.ui.ipconnect 2.0

Popup { id: setting

	property string theme: IPConnect.theme
	property alias username: userName.text
	property alias ddir: downloadDirVal.text
	property alias themeColor: colorDialog.color

	signal updated()

	modal: true
	height: parent.height
	width: parent.width
	closePolicy: Popup.NoAutoClose

	Component.onCompleted: {
		open()
		welcomeTimer.start()
	}

	contentItem: Rectangle{

		Timer { id: welcomeTimer
			running: false
			interval: 1000
			repeat: false
			onTriggered: {
				welcomeText.opacity = 0
				settingForm.visible = true
			}
		}

		Text { id: welcomeText
			text: qsTr("Welcome To IPConnect")
			color: setting.theme
			font.pixelSize: 30
			anchors.centerIn: parent
			Behavior on opacity { NumberAnimation { duration: 500 } }
		}

		Item { id: settingForm
			anchors.fill: parent
			visible: false

			Behavior on visible { NumberAnimation { duration: 600 } }

			Text { id: settingsTitle
				height: 40
				anchors.top: parent.top
				anchors.horizontalCenter: parent.horizontalCenter
				font.pixelSize: 20
				color: setting.theme
				text: qsTr("Configure Settings")
			}

			SwipeView { id: inputs

				clip: true
				anchors.centerIn: parent
				width: 400
				height: 400
				interactive: false

				Page{

					Item{
						anchors.centerIn: parent
						width: 300
						height: userName.height + userNameLabel.height

						Column {
							anchors.fill: parent

							Text { id: userNameLabel
								text: qsTr("User Name")
								font.pixelSize: 14
								height: 30
								width: parent.width
								Behavior on font.pixelSize { NumberAnimation { duration: 100 } }
							}

							TextField { id: userName
								font.pixelSize: 14
								placeholderText: qsTr("ex: rsnake")
								height: 40
								width: parent.width
								onActiveFocusChanged: userNameLabel.font.pixelSize = 20
							}
						}
					}
				}

				Page{
					Item { id: downloadDirForm
						anchors.centerIn: parent
						width: 300
						height: downloadDirLabel.height + downloadDir.height

						Column {
							anchors.fill: parent

							Text { id: downloadDirLabel
								text: qsTr("Default Download Directory")
								font.pixelSize: 14
								height: 40
								width: parent.width
								Behavior on font.pixelSize { NumberAnimation { duration: 100 } }
							}

							Item { id: downloadDir
								width: parent.width
								height: 40

								TextField { id: downloadDirVal
									anchors.left: parent.left
									anchors.right: selectBtn.left
									height: parent.height
									placeholderText: qsTr("ex: ~/Downloads")
									text: dirDialog.folder
									onActiveFocusChanged:
										if(downloadDirLabel.font.pixelSize != 20)
											downloadDirLabel.font.pixelSize = 20
								}

								Button { id: selectBtn
									anchors.right: parent.right
									text: qsTr("\uf07b")
									height: parent.height
									width: height
									onClicked: {
										if(downloadDirLabel.font.pixelSize != 20)
											downloadDirLabel.font.pixelSize = 20
										dirDialog.open()
									}
								}
							}
						}
						FileDialog { id: dirDialog
							selectExisting: true
							selectFolder: true
						}
					}
				}

				Page{
					ColorDialog { id: colorDialog
						currentColor: setting.theme
						color: currentColor
					}
					Item { id: colorThemeForm
						anchors.centerIn: parent
						width: 140
						height: colorThemeLabel.height + colorView.height

						Column {
							anchors.fill: parent


							Text { id: colorThemeLabel
								text: qsTr("Choose Color Theme")
								font.pixelSize: 14
								height: 40
								width: parent.width
								Behavior on font.pixelSize { NumberAnimation { duration: 100 } }
							}

							Rectangle { id: colorView
								anchors.horizontalCenter: parent.horizontalCenter
								color: colorDialog.color
								width: 100
								height: 100

								Text { id: colorText
									anchors.centerIn: parent
									text: colorDialog.color
								}

								MouseArea{
									anchors.fill: parent
									onClicked: {
										colorThemeLabel.font.pixelSize = 20
										colorDialog.open()
									}
								}
							}
						}
					}
				}
			}

			Item { id: btngrp1
				width: 400
				height: 50
				anchors.top: inputs.bottom
				anchors.horizontalCenter: parent.horizontalCenter

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
								setting.updated()
								setting.close()
							}
						}
					}
				}
			}
		}
	}
}
