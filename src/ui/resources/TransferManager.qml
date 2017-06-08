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
import QtGraphicalEffects 1.0
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

			Item { id: pTWrapper
				width: parent.width
				height: pTRect.height
				Rectangle { id: pTRect
					clip: true
					width: parent.width - 20
					height: infoCol.height + actionRow.height
					anchors.horizontalCenter: parent.horizontalCenter
					radius: 2

					Column { id: infoCol
						width: parent.width
						height: pTFileName.height + pTFileSize.height + pTFrom.height

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
					Row { id: actionRow
						width: parent.width
						height: 50
						anchors.top: infoCol.bottom
						Rectangle { id: acceptBtn
							color: "#4285f4"
							width: 100
							height: 40
							anchors.verticalCenter: parent.verticalCenter
							anchors.left: parent.left
							Text {
								color: "#fdfdfd"
								text: qsTr("Accept")
								anchors.centerIn: parent
							}
						}
						Rectangle { id: rejectBtn
							color: "#4285f4"
							width: 100
							height: 40
							anchors.verticalCenter: parent.verticalCenter
							anchors.left: acceptBtn.right
							Text {
								color: "#fdfdfd"
								text: qsTr("Reject")
								anchors.centerIn: parent
							}
						}
					}
				}
				DropShadow { 
					anchors.fill: pTRect
					source: pTRect
					horizontalOffset: 0
					verticalOffset: 1
					radius: 6
					samples: 32
					color: "#60000000"
					transparentBorder: true
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
