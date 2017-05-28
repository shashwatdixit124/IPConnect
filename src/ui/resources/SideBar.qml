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

import QtQuick 2.0
import QtGraphicalEffects 1.0
import api.ui.ipconnect 2.0

Item { id: item
	width: 60
	height: parent.height
	anchors.left: parent.left

	property string theme: IPConnect.theme
	property alias title : applicationNameText.text
	property real tabHeight : 60

	signal messengerClicked
	signal settingsClicked
	signal quickConnectClicked
	signal fileTransferClicked

	FontLoader { id: awesome; source: "qrc:/resources/fontawesome-webfont.ttf" }

	Item { id:sidePane
		anchors.fill: parent

		Column{
			anchors.fill: parent

			Item { id: applicationName
				width: parent.width
				height: item.tabHeight

				Text {
					id: applicationNameText
					color: item.theme
					font.pixelSize: 24
					anchors.centerIn: parent
				}
			}

			Item { id: messenger
				width: parent.width
				height: item.tabHeight

				Text { id: messengerIcon
					anchors.centerIn: parent
					text: qsTr("\uf075")
					color: item.theme
					font.pixelSize: 24
				}

				MouseArea{
					anchors.fill: parent
					cursorShape: Qt.PointingHandCursor
					onClicked: messengerClicked()
				}
			}

			Item { id: settings
				width: parent.width
				height: item.tabHeight

				Text { id: settingsIcon
					anchors.centerIn: parent
					text: qsTr("\uf013")
					color: item.theme
					font.pixelSize: 24
				}

				MouseArea{
					anchors.fill: parent
					cursorShape: Qt.PointingHandCursor
					onClicked: settingsClicked()
				}
			}

			Item { id: quickConnect
				width: parent.width
				height: item.tabHeight

				Text { id: quickConnectIcon
					anchors.centerIn: parent
					text: qsTr("\uf0c1")
					color: item.theme
					font.pixelSize: 24
				}

				MouseArea{
					anchors.fill: parent
					cursorShape: Qt.PointingHandCursor
					onClicked: quickConnectClicked()
				}
			}

			Item { id: fileTransfer
				width: parent.width
				height: item.tabHeight

				Text { id: fileTransferIcon
					anchors.centerIn: parent
					text: qsTr("\uf15b")
					color: item.theme
					font.pixelSize: 24
				}

				MouseArea{
					anchors.fill: parent
					cursorShape: Qt.PointingHandCursor
					onClicked: fileTransferClicked()
				}
			}
		}
	}
}
