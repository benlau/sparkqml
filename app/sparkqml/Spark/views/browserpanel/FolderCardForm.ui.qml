import QtQuick 2.4
import FontAwesome 1.0
import "../../constants"

Item {
    id: component
    width: 180
    height: 40

    property string folder: "Folder"

    Card {
        id: card1
        anchors.fill: parent

        Rectangle {
            id: rectangle1
            width: 40
            height: 40
            color: "#f9f9f9"

            Text {
                anchors.centerIn: parent
                font.family: FontAwesome.fontFamily
                text: FontAwesome.folder
                font.pointSize: 18
                color: Constants.black54
            }
        }

        Text {
            id: text1
            verticalAlignment: Text.AlignVCenter
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 0
            anchors.top: parent.top
            anchors.topMargin: 0
            anchors.right: parent.right
            anchors.rightMargin: 8
            anchors.left: parent.left
            anchors.leftMargin: 48
            font.pixelSize: 16
            color: Constants.black54
            elide: Text.ElideRight
            text: component.folder
        }
    }


}
