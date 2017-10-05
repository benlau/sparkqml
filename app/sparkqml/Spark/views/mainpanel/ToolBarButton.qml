import QtQuick 2.4
import QtQuick.Controls 2.1
import FontAwesome 1.0

Button {
    id: component
    width: 32
    implicitWidth: 32

    height: 24
    implicitHeight: 24

    hoverEnabled: true

    property string toolTip: ""

    property string iconSource: FontAwesome.tasks

    baselineOffset: 0

    ToolTip.delay: 1000

    ToolTip.timeout: 5000

    ToolTip.text: toolTip

    ToolTip.visible: component.hovered && toolTip !== ""

    topPadding: 0
    leftPadding: 0
    rightPadding: 0
    bottomPadding: 0

    contentItem: Text {
        text: component.iconSource
        color: "#deffffff"
        font.pixelSize: 14
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        anchors.fill: parent
        font.family: FontAwesome.fontFamily
    }

    background: Rectangle {
        id: mask
        anchors.fill: parent
        color: component.down ? "#4CFFFFFF" : "transparent"
    }

}
