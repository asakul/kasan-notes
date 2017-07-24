import QtQuick 2.0

Rectangle {
	anchors.fill: parent
	TextEdit {
		id: textEdit
		anchors.fill: parent
		objectName: "NoteText"
		textFormat: TextEdit.RichText
		selectByMouse: true
	}
	
	function setText(text) {
		textEdit.text = text
	}
}
