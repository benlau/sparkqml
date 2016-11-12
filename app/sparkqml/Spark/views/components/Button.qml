import QtQuick 2.0
import QtQuick.Controls 2.0

FocusScope {
    id: component

    property alias hoverEnabled: mouseArea.hoverEnabled

    property string toolTip: ""

    property alias pressed: mouseArea.pressed

    signal clicked()

    ToolTip.delay: 1000

    ToolTip.timeout: 5000

    ToolTip.visible: mouseArea.hovered && toolTip !== ""

    ToolTip.text: toolTip

    MouseArea {
        id: mouseArea
        anchors.fill: parent

        property bool hovered: false

        onClicked: {
            component.clicked();
        }

        onEntered: hovered = true;

        onExited: hovered = false;
    }

}
