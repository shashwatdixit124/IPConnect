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
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import api.ui.ipconnect 2.0

Item { id: item

	property string theme: IPConnect.theme
	property string textColor: "#333"
	property string bgColor: "#f7f7f7"

	Item { id: pendingTransfers
		anchors.fill: parent

		Text { id: emptyPTText
			text: qsTr("Empty")
			anchors.centerIn: parent
			font.pixelSize: 80
			font.weight: Font.Light
			color: "#10000000"
			visible: pTList.currentIndex > -1 ? false : true
		}

		Component { id: pTDelegate

			Rectangle { id: pTWrapper
				border.width: 1
				border.color: item.theme
				width: parent.width - 20
				height: pTFileName.height + pTFileSize.height + pTFrom.height
				anchors.horizontalCenter: parent.horizontalCenter

				Column { 
					anchors.fill: parent

					Item { id: pTFileName
						width: parent.width - 20
						height: 30
						anchors.horizontalCenter: parent.horizontalCenter
						Text { 
							anchors.verticalCenter: parent.verticalCenter
							text: qsTr("<b>File </b> : "+filename)
						}
					}

					Item { id: pTFileSize
						width: parent.width - 20
						height: 30
						anchors.horizontalCenter: parent.horizontalCenter
						Text { 
							anchors.verticalCenter: parent.verticalCenter
							text: qsTr("<b>Size </b> : "+filesize)
						}
					}

					Item { id: pTFrom
						width: parent.width - 20
						height: 30
						anchors.horizontalCenter: parent.horizontalCenter
						Text { 
							anchors.verticalCenter: parent.verticalCenter
							text: qsTr("<b>From </b> : "+clientname)
						}
					}
				}
			}
		}

		ScrollView {
			anchors.fill: parent
			flickableItem.interactive: true

			style: ScrollViewStyle {
				transientScrollBars: true
				handle: Item {
					implicitWidth: 14
					implicitHeight: 26
					Rectangle {
						color: "#50000000"
						anchors.fill: parent
						anchors.margins: 4
						radius: 4
					}
				}
			}

			ListView { id:pTList
				anchors.fill: parent
				anchors.topMargin: 5
				delegate: pTDelegate
				model: IPConnect.pendingTransfers
				spacing: 5
			}
		}

	}

}
