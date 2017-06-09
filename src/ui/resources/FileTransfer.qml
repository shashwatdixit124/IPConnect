/*
 *   This file is part of IPConnect
 *   Copyright 2017 Shashwat Dixit <shashwatdixit124@gmail.com>
 * 
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU Library General Public License as
 *   published by the Free Software Foundation; either version 3 of the
 *   License, or (at your option) any later version.
 * 
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 * 
 *   You should have received a copy of the GNU Library General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

import QtQuick 2.5
import QtQuick.Controls 2.1
import QtQuick.Dialogs 1.2
import QtGraphicalEffects 1.0
import api.ui.ipconnect 2.0

Popup { id: item
	height: 200
	width: 400
	x: (parent.width - 60 - width)/2
	y: (parent.height - height)/2
	modal: true

	FontLoader { id: awesome; source: "qrc:/resources/fontawesome-webfont.ttf" }

	background: Item{
		implicitHeight: 200
		implicitWidth: 400
		Rectangle { id: popupBack
			anchors.fill: parent
		}
		DropShadow { 
			anchors.fill: popupBack
			source: popupBack
			horizontalOffset: 0
			verticalOffset: 0
			radius: 16
			samples: 32
			color: "#60000000"
			transparentBorder: true
		}
	}

	contentItem: Item{
		anchors.fill: parent
		Text { id: fileTransferText
			text: qsTr("Send File")
			font.pixelSize: 24
			anchors.top: parent.top
			anchors.topMargin: 10
			anchors.horizontalCenter: parent.horizontalCenter
		}

		Rectangle{ id: fileTransferForm
			anchors.horizontalCenter: parent.horizontalCenter
			anchors.bottom: parent.bottom
			anchors.bottomMargin: 20
			height: fileSelectForm.height+sendFileForm.height
			width: filePathField.width + fileTransferBtn.width

			FileDialog{
				id: sendFileDialog
				selectFolder: false
				selectExisting: true
			}

			Column{
				anchors.fill: parent

				Row { id: fileSelectForm
					width: parent.width
					height: 40
					TextField { id: filePathField
						height: parent.height
						width: 300
						placeholderText: qsTr(" File Location")
						text: sendFileDialog.fileUrl
					}
					IPCButton {id: fileTransferBtn
						icon: qsTr("\uf15b")
						radius: 0
						shadow: false
						onClicked:{
							sendFileDialog.open()
						}
					}
				}
				Item { id: sendFileForm
					width:  parent.width
					height: 60

					IPCButton { id: sendFileBtn
						anchors.bottom: parent.bottom
						anchors.right: parent.right
						icon: qsTr("\uf1d8")
						iconFont: awesome.name
						text: qsTr("Send")
						onClicked: {
							IPConnect.sendFile(filePathField.text);
							item.close()
						}
					}
				}
			}
		}
	}
}
