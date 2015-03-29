
import QtQuick 2.4

ListView {
  Component {
    id: objRecursiveDelegate
    Column {
       id: objRecursiveColumn
       clip: true
       MouseArea {
          width: objRow.implicitWidth
          height: objRow.implicitHeight
          onClicked: {
             for(var i = 1; i < parent.children.length - 1; ++i) {
                parent.children[i].visible = !parent.children[i].visible
             }
             objText.text = (objRecursiveColumn.children.length > 2 ?
                            objRecursiveColumn.children[1].visible ?
                            qsTr("-  ") : qsTr("+ ") : qsTr("   ")) + model.name
          }
          Row {
             id: objRow
             Item {
                height: 1
                width: model.level * 10
             }
             Text {
                id: objText
                text: (objRecursiveColumn.children.length > 2 ?
                            objRecursiveColumn.children[1].visible ?
                            qsTr("-  ") : qsTr("+ ") : qsTr("   ")) + model.name
                color: objRecursiveColumn.children.length > 2 ? "#303030" : "black"
                font { pixelSize: 12 }
             }
          }
       }
       Repeater {
          model: subNode
          delegate: objRecursiveDelegate
       }
    }
  }

  delegate: objRecursiveDelegate
}
