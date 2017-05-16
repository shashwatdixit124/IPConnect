import QtQuick 2.0
import QtQuick.Controls 2.0

Item {
    id:item

    property bool dark
    property string textColor
    property string bgColor

	Messenger{
		id: messenger
        anchors.fill: parent        
        dark: item.dark
        textColor: item.textColor
        bgColor: item.bgColor
	}
	
}
