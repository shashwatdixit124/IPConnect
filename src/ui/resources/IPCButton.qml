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

Item { id: btnRoot
	signal clicked
	property string icon
	property string textColor : "#fbfbfb"
	property bool shadow : true

	property alias text : btnText.text
	property alias color : btnBack.color
	property alias radius: btnBack.radius
	property alias iconFont: btnIcon.font
	property alias shadowColor: dropBtnBack.color

	height: 40
	width: content.width <  height ? height : content.width + 20

	Rectangle { id: btnBack
		anchors.fill: parent
		color: IPConnect.theme
		radius: 4
		clip: true

		Row { id: content
			height: parent.height
			width: iconBlk.width + textBlk.width
			anchors.horizontalCenter: parent.horizontalCenter
			Item { id: iconBlk
				width: 30
				height: parent.height
				Text { id: btnIcon
					text: icon
					font.pixelSize: 16
					font.family: iconFont
					anchors.centerIn: parent
					color: btnRoot.textColor
				}
			}
			Item { id: textBlk
				width: btnText.implicitWidth
				height: parent.height
				Text { id: btnText
					font.weight: Font.Bold
					font.pixelSize: 12
					anchors.verticalCenter: parent.verticalCenter
					color: btnRoot.textColor
				}
			}
		}

		MouseArea { id: btnArea
			anchors.fill: parent
			cursorShape: Qt.PointingHandCursor
			onClicked: {
				btnRoot.clicked()
			}
		}
	}

	DropShadow { id: dropBtnBack
		anchors.fill: btnBack
		source: btnBack
		horizontalOffset: 0
		verticalOffset: 0
		radius: 8
		samples: 32
		color: "#30000000"
		transparentBorder: true
		visible: btnRoot.visible
	}
}
