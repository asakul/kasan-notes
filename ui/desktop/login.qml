import QtQuick 2.4

Rectangle {
	id: loginForm

	width: 400
	height: 400
	color: "#404244"
	
	signal doLogin(string login, string password)
	
	Component.onCompleted: {
		submit.clicked.connect(emitLoginSignal)
	}
	
	function emitLoginSignal() {
		doLogin(login.text, password.text)
	}

	Keys.onPressed: {
		if(event.key == Qt.Key_Return) {
			emitLoginSignal();
		}
	}

	Column {	
		anchors.centerIn: parent
		spacing: 10
		
		Column {
			spacing: 4
			MediumText { text: "Login" }
			LineInput {
				id: login
				focus: true
				width: 180; height: 28
				
				KeyNavigation.tab: password
			}
		}
		
		Column {
			spacing: 4
			MediumText { text: "Password" }
			LineInput {
				id: password
				input.echoMode: TextInput.Password
				width: 180; height: 28
				
				KeyNavigation.backtab: login
			}
		}
		
		Row {
			spacing: 10
			anchors.horizontalCenter: parent.horizontalCenter
			Button {
				id: submit
				text: "OK"
			}
		}
		
	}
}
