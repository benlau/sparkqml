import QtQuick 2.0
import QtQuick.Layouts 1.1
import FontAwesome 1.0
import "../../constants"

Item {
    id: component

    height: 24

    implicitWidth: button.implicitWidth + separator.width + 8

    property bool showSeparator: true

    signal clicked

    property alias text: button.text

    RowLayout {
        id: row
        spacing: 0
        anchors.fill: parent

        PathButton {
            id: button
            onClicked: {
                component.clicked();
            }

            Layout.fillWidth: true
            Layout.maximumWidth: button.implicitWidth
        }

        Item {
            Layout.fillWidth: true
            Layout.maximumWidth: separator.width + 8
            Layout.fillHeight: true

            Text {
                id: separator
                font.family: FontAwesome.fontFamily
                text: showSeparator ? FontAwesome.chevronRight : ""
                verticalAlignment: Text.AlignVCenter
                font.pixelSize: 10
                height: 24
                color: Constants.black54
                anchors.centerIn: parent
            }
        }


    }


}
