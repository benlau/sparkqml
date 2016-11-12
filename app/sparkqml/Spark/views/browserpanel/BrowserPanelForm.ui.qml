import QtQuick 2.4
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0

Item {
    width: 400
    height: 400

    ColumnLayout {
        id: columnLayout1
        spacing: 0
        anchors.fill: parent

        BrowserActionBar {
            z: 1
            Layout.alignment: Qt.AlignLeft | Qt.AlignTop
            Layout.fillWidth: true
        }

        Rectangle {
            id: componentGrid
            color: "#eaeaea"
            Layout.fillWidth: true
            Layout.fillHeight: true

        }

        Rectangle {
            id: divider
            width: 200
            height: 200
            color: "#bfbfbf"
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.maximumHeight: 1
        }

        Rectangle {
            id: browserFooterBar
            width: 200
            height: 42
            color: "#e8e9e8"
            Layout.fillHeight: true
            Layout.maximumHeight: 42
            Layout.alignment: Qt.AlignLeft | Qt.AlignBottom
            Layout.fillWidth: true
        }

    }
}
