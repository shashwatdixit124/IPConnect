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
import api.ui.ipconnect 2.0

Item { id:item
	property string textColor
	property string bgColor
	property SideBar sideBar

	Connections{
		target: sideBar
		onMessengerClicked: {
			messenger.visible = true
			transferManager.visible = false
			appSettings.visible = false
		}
		onTransferManagerClicked: {
			messenger.visible = false
			transferManager.visible = true
			appSettings.visible = false
		}
		onSettingsClicked: {
			messenger.visible = false
			transferManager.visible = false
			appSettings.visible = true
		}
		onQuickConnectClicked: {
			quickConnect.open()
		}
		onFileTransferClicked: {
			fileTransfer.open()
		}
	}

	Messenger { id: messenger
		anchors.margins: 20
		anchors.fill: parent
		textColor: item.textColor
		bgColor: item.bgColor
	}

	TransferManager { id: transferManager
		anchors.margins: 20
		anchors.fill: parent
		textColor: item.textColor
		bgColor: item.bgColor
		visible: false
	}

	AppSettings { id: appSettings
		anchors.margins: 20
		anchors.fill: parent
		textColor: item.textColor
		bgColor: item.bgColor
		visible: false
	}

	QuickConnect{ id: quickConnect }

	FileTransfer{ id: fileTransfer }

	Notification { id: notification
		text: IPConnect.notificationMsg
		status: IPConnect.notificationStatus
	}
}
