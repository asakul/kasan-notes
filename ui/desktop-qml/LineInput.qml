import QtQuick 2.4

FocusScope {
	property alias input: input
	property alias text: input.text
	x: item.x
	y: item.y
	width: item.width
	height: item.height

Item {
	id: item
	property alias text: input.text
	property alias input: input
	
	Rectangle {
		id: container
		border.width: 2
		border.color: focus ? "#6666dd" : "#666666"
		width: 180; height: 28
		TextInput {
			id: input
			color: "#151515"; selectionColor: "green"
			font.pixelSize: 16; font.bold: true
			width: parent.width-16
			anchors.centerIn: parent
			focus: true
		}
	}
}

}