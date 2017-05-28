import QtQuick 2.5
import QtQuick.Controls 2.1
import QtQuick.Dialogs 1.2

Popup { id: item
	height: 200
	width: 400
	x: (parent.width - 60 - width)/2
	y: (parent.height - height)/2
	modal: true

	FontLoader { id: awesome; source: "qrc:/resources/fontawesome-webfont.ttf" }

	contentItem: Item{
		anchors.fill: parent
		Text { id: fileTransferText
			text: qsTr("Send File")
			font.pixelSize: 24
			anchors.top: parent.top
			anchors.topMargin: 10
			anchors.horizontalCenter: parent.horizontalCenter
		}

		Item { id: fileTransferForm
			anchors.centerIn: parent
			height: fileSelectForm.height+sendFileForm.height
			width: fileTransferUrl.width + fileTransferBtn.width

			FileDialog{
				id: sendFileDialog
				selectFolder: false
				selectExisting: true
				onFileUrlChanged: console.log("file selected : "+fileUrl)
			}

			Column{
				anchors.fill: parent

				Row { id: fileSelectForm
					width: parent.width
					height: 40
					TextField { id: fileTransferUrl
						height: parent.height
						width: 300
						placeholderText: qsTr(" File Location")
						text: sendFileDialog.fileUrl
					}
					Button { id: fileTransferBtn
						height: parent.height
						width: height
						text: qsTr("\uf15b")
						onClicked:{
							fileTransferUrl.text = ""
							sendFileDialog.open()
						}
					}
				}
				Item { id: sendFileForm
					width:  parent.width
					height: 60

					Button { id: sendFileBtn
						height: 40
						width: 100
						anchors.bottom: parent.bottom
						anchors.right: parent.right
						text: qsTr("\uf1d8  Send")
						onClicked: {
							//TODO Connect API send function
							item.close()
						}
					}
				}
			}
		}
	}
}
