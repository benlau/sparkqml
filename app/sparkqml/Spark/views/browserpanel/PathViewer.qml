import QtQuick 2.0
import QtQuick.Layouts 1.1

Item {
    id: component

    height: 36

    property var path : new Array

    RowLayout {
        anchors.fill: parent
        spacing: 0

        Repeater {
            model: path

            delegate: PathItem {
                id: item
                height: 24
                text: modelData
                showSeparator: index !== path.length - 1
                visible: index > path.length - 4

                Layout.alignment: Qt.AlignLeft
                Layout.fillWidth: true
                Layout.maximumWidth: item.implicitWidth
            }
        }

        Item {
            Layout.fillWidth: true
            Layout.fillHeight:true
        }

    }

}
