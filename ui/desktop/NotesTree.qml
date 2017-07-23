
import QtQuick 2.4
import QtQuick.Controls 1.4

TreeView {
  id: notesTreeView

  headerVisible: false

  TableViewColumn {
  	title: ""
  	role: "display"
  }
  
  model: notesModel
}
