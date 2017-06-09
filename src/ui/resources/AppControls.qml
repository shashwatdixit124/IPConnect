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

Item { id: root
	property string theme: "#bbb"//IPConnect.theme
	property string textColor : "#333"
	property string bgColor : "#fdfdfd"

	signal close()
	signal minimized()
	signal maximized()
	signal drag(real dragX,real dragY)

	FontLoader { id: awesome; source: "qrc:/resources/fontawesome-webfont.ttf" }

	Item { id: titleBar
		height:parent.height
		width: parent.width

		MouseArea { id: titleRegion
			anchors.fill: parent;
			cursorShape: Qt.DragMoveCursor
			property variant clickPos: "1,1"

			onPressed: {
				clickPos  = Qt.point(mouse.x,mouse.y)
			}

			onPositionChanged: root.drag(mouse.x-clickPos.x,mouse.y-clickPos.y)
		}

		Item { id:titleBarToolsMinimize
			height: parent.height
			width: parent.height
			anchors.right: titleBarToolsMaximize.left

			Text { id: minimizeBtn
				font.pixelSize: 12
				font.family: awesome.name
				color: root.theme
				text: "\uf068"
				anchors.centerIn: parent
			}

			MouseArea { id: minimizeRegion
				cursorShape: Qt.PointingHandCursor
				anchors.fill: minimizeBtn
				onPressed: root.minimized()
			}
		}

		Item { id:titleBarToolsMaximize
			height: parent.height
			width: parent.height
			anchors.right: titleBarToolsClose.left

			Text{ id: maximizeBtn
				font.pixelSize: 12
				font.family: awesome.name
				color: root.theme
				text: "\uf065"
				anchors.centerIn: parent
			}

			MouseArea{ id: maximizeRegion
				cursorShape: Qt.PointingHandCursor
				anchors.fill: maximizeBtn
				onPressed: root.maximized()
			}
		}

		Item { id:titleBarToolsClose
			height: parent.height
			width: parent.height
			anchors.right: parent.right

			Text { id: closeBtn
				font.pixelSize: 12
				font.family: awesome.name
				color: root.theme
				text: "\uf00d"
				anchors.centerIn: parent
			}

			MouseArea { id:closeRegion
				cursorShape: Qt.PointingHandCursor
				anchors.fill: closeBtn
				onPressed: root.close()
			}
		}
	}
}

