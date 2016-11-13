import QtQuick 2.4
import QtQuick.Layouts 1.3
import "../components"

Item {
    width: 180
    height: 180

    property alias source: viewer.source

    ColumnLayout {
        spacing: 0
        anchors.fill: parent

        Card {
            width: 180
            height: 140

            ComponentViewer {
                id: viewer
                anchors.fill: parent
            }
        }

        Text {
            id: text1
            color: "#de000000"
            text: qsTr("Component.qml")
            verticalAlignment: Text.AlignVCenter
            Layout.fillWidth: true
            Layout.fillHeight: true
            horizontalAlignment: Text.AlignHCenter
            font.pixelSize: 14
            elide: Text.ElideMiddle
        }

        Text {
            id: text2
            color: "#de000000"
            text: qsTr("ComponentForm.ui.qml")
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            Layout.fillHeight: true
            Layout.fillWidth: true
            font.pixelSize: 14
            elide: Text.ElideMiddle
        }


    }

}
