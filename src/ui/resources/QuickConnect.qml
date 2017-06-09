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

	contentItem: Item { 
		anchors.fill: parent
		Text { id: quickConnectText
			text: qsTr("Quick Connect")
			font.pixelSize: 24
			anchors.top: parent.top
			anchors.topMargin: 10
			anchors.horizontalCenter: parent.horizontalCenter
		}

		Item { id: quickConnectForm
			anchors.centerIn: parent
			height: 40
			width: quickConnectUrl.width + quickConnectBtn.width

			Row {
				anchors.fill: parent
				TextField { id: quickConnectUrl
					height: parent.height
					width: 300
					placeholderText: qsTr(" URL")
					onAccepted:{
						IPConnect.quickConnect(quickConnectUrl.text)
						quickConnectUrl.text = ""
						close()
					}
				}
				IPCButton { id: quickConnectBtn
					shadow: false
					radius: 0
					icon: qsTr("\uf0c1")
					onClicked:{
						IPConnect.quickConnect(quickConnectUrl.text)
						quickConnectUrl.text = ""
						close()
					}
				}
			}
		}
	}
}
