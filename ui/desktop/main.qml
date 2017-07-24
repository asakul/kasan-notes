
import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Layouts 1.1

Rectangle {
	id: mainForm

	color: "#404244"

	RowLayout {
		spacing: 1
		anchors.fill: parent
		anchors.margins: 1
		Layout.fillHeight: true

		KasanToolBar {
			id: toolbar

			objectName: "toolbar"

			Layout.fillHeight: true
			Layout.preferredWidth: 60
		}

		Item {
			Layout.fillHeight: true
			Layout.fillWidth: true
			SplitView {
				anchors.fill: parent
				orientation: Qt.Horizontal

				Rectangle {
					width: 200
					Layout.maximumWidth: 400
					color: "#ededed"

					NotesTree {
						id: notes
						objectName: "notes"
						anchors.fill: parent
					}
				}

				Rectangle {
					id: noteArea
					objectName: "NoteArea"
					Layout.fillWidth: true
					Layout.minimumWidth: 200
					color: "lightgray"

					Text {
						text: "Main panel"
					}
				}
			}
		}
	}
}
