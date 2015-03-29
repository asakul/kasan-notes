
import QtQuick 2.4
import QtQuick.Controls 2.4

ApplicationWindow {
	id: mainForm

	color: "#404244"
	
	SplitView {
		anchors.fill: parent
		orientation: Qt.Horizontal

		Rectangle {
			width: 200
			Layout.maximumWidth: 400
			color: "#ededed"

		}

		Rectangle {
			id: noteArea

			Layout.minimumWidth: 200
			color: "lightgray"
		}
	}
}
