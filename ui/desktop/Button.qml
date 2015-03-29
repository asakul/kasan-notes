import QtQuick 2.4
Item {
    id: container
    
    width: label.width+32; height: label.height+16
    signal clicked
    property alias text: label.text
    
    Rectangle {
	    width: container.width; height: container.height
	    border.width: 1
	    border.color: "#666066"
	    color: "#444444"
    }
    Text {
	    id: label
	    color: region.pressed? "#FFFFFF" : "#DDDDDD"
	    anchors.centerIn: container; font.bold: true
	    style: Text.Raised; styleColor: "black"
	    font.pixelSize: 12
    }
    MouseArea {
	    id: region
	    anchors.fill: container
	    onClicked: container.clicked()
    }
    transitions: Transition {
   		ColorAnimation { target: label; }
    }
}