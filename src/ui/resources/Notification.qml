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

Item { id: notification

	property alias text: notificationMsg.text

	Behavior on visible {NumberAnimation {duration: 300} }
	Behavior on opacity {NumberAnimation {duration: 200} }
	clip: true

	height: 220
	width: 320
	x: parent.width - width - 20
	y: parent.height - height

	FontLoader { id: awesome; source: "qrc:/resources/fontawesome-webfont.ttf" }

	states: [
		State {
			name: "Active"
			PropertyChanges {
				target: notification
				opacity: 1
				visible: true
			}
		},
		State {
			name: "Inactive"
			PropertyChanges {
				target: notification
				opacity: 0
				visible: false
			}
		}
	]

	Rectangle { id: notificationItem
		radius: 5
		height: 200
		width: 300
		anchors.centerIn: parent

		Item{
			anchors.fill: parent
			Item { id: closeItem
				height: 40
				width: height
				anchors.top: parent.top
				anchors.right: parent.right
				Text { 
					text: qsTr("\uf00d")
					font.pixelSize: 16
					anchors.centerIn: parent
					color: "#444"
				}
				MouseArea{
					anchors.fill: parent
					cursorShape: Qt.PointingHandCursor
					onClicked: notification.state = "Inactive"
				}
			}
			TextArea { id: notificationMsg
				font.weight: Font.Light
				font.pixelSize: 16
				wrapMode: TextArea.WordWrap
				width: parent.width - 20
				anchors.horizontalCenter: parent.horizontalCenter
				anchors.bottom: parent.border
				anchors.top: closeItem.bottom
				anchors.margins: 10
				readOnly: true
			}
		}

	}

	DropShadow { 
		anchors.fill: notificationItem
		source: notificationItem
		horizontalOffset: 0
		verticalOffset: 0
		radius: 16
		samples: 32
		color: "#20000000"
		transparentBorder: true
	}
}
