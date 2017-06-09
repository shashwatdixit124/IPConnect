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

import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Window 2.2
import QtGraphicalEffects 1.0
import api.ui.ipconnect 2.0

ApplicationWindow { id:root
	visible: true
	minimumWidth: 800
	minimumHeight: 600
	x:(Screen.width-800)/2
	y:(Screen.height-600)/2
	flags: Qt.FramelessWindowHint

	property string textColor:  "#333"
	property string rootBgColor: "#fcfcfc"
	property string sbBgColor: "#f6f6f6"
	property string sbTextColor: "#333"

	Welcome{
		onUpdated: {
			IPConnect.userName = username
			IPConnect.downloadDir = ddir
			IPConnect.theme = themeColor
		}
	}

	Item {
		anchors.fill: parent

		SideBar{ id: sb
			title: qsTr("𝓲𝓹𝓬")
		}

		Rectangle { id:window
			height: parent.height
			anchors.left: sb.right
			anchors.right: parent.right
			color: root.rootBgColor

			AppControls { id: appControls
				anchors.top:parent.top
				width: parent.width
				height: 40
				onClose: Qt.quit()
				onMaximized: root.showMaximized()
				onMinimized: root.showMinimized()
				onDrag: {
					root.x += dragX
					root.y += dragY
				}
			}

			MainView { id:mainView
				textColor: root.textColor
				bgColor: root.rootBgColor
				sideBar: sb
				anchors.left: parent.left
				anchors.right: parent.right
				anchors.top: appControls.bottom
				anchors.bottom: statusBar.top
			}

			StatusBar { id:statusBar
				width:parent.width
				height: 24
				anchors.bottom: parent.bottom
				textColor: root.sbTextColor
				bgColor: root.sbBgColor
				onSizeChanged: {
					root.width += sizeX
					root.height += sizeY
				}
			}
		}

		InnerShadow {
			anchors.fill: window
			cached: true
			horizontalOffset: 1
			verticalOffset: 0
			radius: 4
			samples: 32
			color: "#10000000"
			smooth: true
			source: window
		}

	}
}

