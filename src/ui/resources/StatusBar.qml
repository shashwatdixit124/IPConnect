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
import QtQuick.Controls 2.0
import QtQuick.Window 2.2
import api.ui.ipconnect 2.0

Item { id:item
	property string theme: IPConnect.theme
	property alias text : statusText.text
	property string textColor
	property string bgColor

	signal sizeChanged(real sizeX, real sizeY)

	Item { id:statusBarWrapper
		anchors.fill: parent

		Item { id:status
			height: parent.height
			anchors.left: parent.left
			anchors.right: resizeRect.left

			Text { id: statusText
				color: item.theme
				x: 10
				anchors.verticalCenter: parent.verticalCenter
				text: item.text
			}
		}

		Image { id:resizeRect
			source: "qrc:/resources/triangle-32.png"
			width: 12
			height: 12
			anchors.right: parent.right
			anchors.bottom: parent.bottom

			MouseArea { id:resizeRegion
				anchors.fill: parent
				cursorShape: Qt.SizeFDiagCursor
				property variant clickPos: "1,1"
				onPressed: {
					clickPos  = Qt.point(mouse.x,mouse.y)
				}
				onPositionChanged: item.sizeChanged(mouse.x-clickPos.x,mouse.y-clickPos.y)
			}
		}
	}
}

