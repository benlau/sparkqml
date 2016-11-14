import QtQuick 2.4
import QtQuick.Layouts 1.3
import "../components"

Item {
    id: component
    width: 180
    height: 180

    property alias source: viewer.source
    property string qml: "Component.qml"
    property string ui: "ComponentForm.ui.qml"

    ColumnLayout {
        spacing: 8
        anchors.fill: parent

        Card {
            width: 180
            height: 140

            ComponentViewer {
                id: viewer
                anchors.fill: parent
                asynchronous: true
                autoScanImportPathList: false
            }
        }

        Text {
            id: text1
            color: "#de000000"
            text: component.qml
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
            text: component.ui
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            Layout.fillHeight: true
            Layout.fillWidth: true
            font.pixelSize: 14
            elide: Text.ElideMiddle
        }



    }

}
