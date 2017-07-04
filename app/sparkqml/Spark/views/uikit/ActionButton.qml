import QtQuick 2.4
import FontAwesome 1.0
import "../components"

Button {
    id: component
    width: 32
    height: 24
    property alias icon:  textItem.text


    Text {
        id: textItem
        color: "#de000000"
        font.pixelSize: 14
        anchors.fill: parent
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        text: FontAwesome.tasks
        font.family: FontAwesome.fontFamily
    }

    Rectangle {
        id: mask
        width: 640
        height: 480
        anchors.fill: parent
        color: "black"
        opacity: 0.1
        visible: component.pressed
    }
}
