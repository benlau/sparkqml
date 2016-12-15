import QtQuick 2.0
import QtQuick.Layouts 1.1
import FontAwesome 1.0

Item {
    id: component

    height: 24

    implicitWidth: button.implicitWidth + separator.implicitWidth + 16

    property bool showSeparator: true

    signal clicked

    property alias text: button.text

    RowLayout {
        id: row
        spacing: 8
        anchors.fill: parent

        PathButton {
            id: button
            onClicked: {
                component.clicked();
            }

            Layout.fillWidth: true
            Layout.maximumWidth: button.implicitWidth
        }

        Text {
            id: separator
            font.family: FontAwesome.fontFamily
            text: showSeparator ? FontAwesome.chevronRight : ""
            verticalAlignment: Text.AlignVCenter
            font.pixelSize: 14
            height: 24
            Layout.fillWidth: true
        }

    }


}
