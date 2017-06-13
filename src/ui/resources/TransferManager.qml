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
import QtQuick.Controls 2.0

Item { id: item

	property string textColor
	property string bgColor

	clip: true

	Item {
		height: parent.height
		width: parent.width > 750 ? 750 : parent.width
		anchors.horizontalCenter: parent.horizontalCenter
		Item {id: tabView
			height: 40
			width: parent.width
			anchors.top: parent.top
			Row {
				clip: true
				anchors.fill: parent
				IPCButton { id: requestedBtn
					width: parent.width / 2
					radius: 0
					active: true
					activeColor: "#fff"
					text: qsTr("Requested")
					onClicked: {
						active = true
						activeBtn.active = false
						pendingTransfer.visible = true
						activeTransfer.visible = false
					}
				}
				IPCButton { id: activeBtn
					width: parent.width / 2
					radius: 0
					active: false
					activeColor: "#fff"
					text: qsTr("Active")
					onClicked: {
						active = true
						requestedBtn.active = false
						pendingTransfer.visible = false
						activeTransfer.visible = true
					}
				}
			}
		}

		Item { id: transferView
			anchors.top: tabView.bottom
			anchors.bottom: parent.bottom
			width: parent.width
			PendingTransfer { id: pendingTransfer
				anchors.fill: parent
				anchors.topMargin: 30
			}
			ActiveTransfer { id: activeTransfer
				anchors.fill: parent
				anchors.topMargin: 30
				visible: false
			}
		}
	}
}
