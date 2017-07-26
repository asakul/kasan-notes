import QtQuick 2.0
import QtQuick.Layouts 1.0
import QtQuick.Controls 1.4

Rectangle {
	id: container
	Layout.minimumWidth: 50

	color: "#ededed"

	signal addButtonClicked()

	Column {
		spacing: 2
		anchors.centerIn: parent

		Rectangle {
			id: addButton

			width: 50
			height: 50

			color: "#ededed"

			border.width: 1

			Image {
				anchors.fill: parent
				anchors.margins: 2
				source : "icons/add-button.png"
				fillMode: Image.PreserveAspectFit
			}

			MouseArea {
				id: addButtonArea
				anchors.fill: parent
				hoverEnabled: true

				onEntered: {
						addButton.color = "#c0c0c0"
				}

				onExited: {
					addButton.color = "#ededed"
				}

				onClicked: {
					container.addButtonClicked()
				}
			}
		}
	}
}
