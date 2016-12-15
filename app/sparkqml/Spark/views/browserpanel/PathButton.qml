import QtQuick 2.4
import FontAwesome 1.0
import "../components"

Button {
    id: component
    height: 24
    implicitWidth: textMetrics.width + 24

    property alias text: textItem.text

    property bool last: false

    Rectangle {
        anchors.fill: parent
        opacity: 0.3
        color: "white"
        visible: component.pressed
    }

    Text {
        id: textItem
        height: 24
        font.pixelSize: 14
        verticalAlignment: Text.AlignVCenter
        anchors.centerIn: parent
        width: parent.width - 16
        elide: Text.ElideRight
    }


    TextMetrics {
        id: textMetrics
        font.pixelSize: textItem.font.pixelSize
        font.family: textItem.font.family
        text: textItem.text
    }


}
