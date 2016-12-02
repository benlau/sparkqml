import QtQuick 2.4
import QtQuick.Controls 2.0
import FontAwesome 1.0
import QtQuick.Layouts 1.3

Rectangle {
    id: rectangle1
    width: 160
    height: 400

    color: "#e7e7e7"
    property alias browse: browse
    property alias openMockup: openMockup
    property alias exitButton: exitButton

    ColumnLayout {
        anchors.fill: parent

        NavigationItem {
            id: browse
            icon: FontAwesome.folderOpenO
            text: "Browse Folder"
            Layout.alignment: Qt.AlignLeft | Qt.AlignTop
        }

        NavigationItem {
            id: openMockup
            icon: FontAwesome.folderOpenO
            text: "Mockup Project"
            Layout.alignment: Qt.AlignLeft | Qt.AlignTop
        }

        Item {
            id: item1
            width: 200
            height: 200
            Layout.fillHeight: true
            Layout.fillWidth: true
        }

        NavigationItem {
            id: exitButton
            Layout.alignment: Qt.AlignLeft | Qt.AlignBottom
        }

    }
}
