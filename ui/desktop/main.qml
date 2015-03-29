
import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Layouts 1.1

Rectangle {
	id: mainForm

	color: "#404244"

	SplitView {
		anchors.fill: parent
		orientation: Qt.Horizontal

		Rectangle {
			width: 200
			Layout.maximumWidth: 400
			color: "#ededed"

			NotesTree {
				id: notes
				anchors.fill: parent
			}
		}

		Rectangle {
			id: noteArea
			Layout.fillWidth: true
			Layout.minimumWidth: 200
			color: "lightgray"

			Text {
				text: "Main panel"
			}
		}
	}
}
