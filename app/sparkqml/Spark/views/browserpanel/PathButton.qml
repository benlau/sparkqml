import QtQuick 2.0
import FontAwesome 1.0
import "../components"

Button {
    id: component
    height: 24
    implicitWidth: row.width + 16

    property alias text: textItem.text

    property bool last: false

    Rectangle {
        anchors.fill: parent
        opacity: 0.3
        color: "white"
        visible: component.pressed
    }

    Row {
        id: row
        anchors.horizontalCenter: parent.horizontalCenter
        spacing: 8

        Text {
            id: textItem
            height: 24
            font.pixelSize: 14
            verticalAlignment: Text.AlignVCenter
        }

        Text {
            font.family: FontAwesome.fontFamily
            text: last ? "" : FontAwesome.chevronRight
            verticalAlignment: Text.AlignVCenter
            font.pixelSize: 14
            height: 24

        }

    }


}
