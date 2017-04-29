import QtQuick 2.4
import QtQuick.Controls 2.1
import FontAwesome 1.0

Button {
    id: component
    width: 32
    height: 24

    hoverEnabled: true

    property string toolTip: ""

    property string icon: FontAwesome.tasks

    ToolTip.delay: 1000

    ToolTip.timeout: 5000

    ToolTip.text: toolTip

    ToolTip.visible: component.hovered && toolTip !== ""

    contentItem: Text {
        text: component.icon
        color: "#deffffff"
        font.pixelSize: 14
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        anchors.centerIn: parent
        font.family: FontAwesome.fontFamily
    }

    background: Rectangle {
        id: mask
        color: component.down ? "#4CFFFFFF" : "transparent"
    }

}
