import QtQuick 2.4
import QtQuick.Controls 2.0
import FontAwesome 1.0
import QtQuick.Layouts 1.3

Rectangle {
    id: rectangle1
    width: 160
    height: 400

    color: "#e7e7e7"
    property alias openMockup: openMockup
    property alias exitButton: exitButton

    ColumnLayout {
        anchors.fill: parent

        NavigationItem {
            id: openMockup
            icon: FontAwesome.folderOpenO
            text: "Mockup Project"
            Layout.alignment: Qt.AlignLeft | Qt.AlignTop
        }

        NavigationItem {
            id: exitButton
            Layout.alignment: Qt.AlignLeft | Qt.AlignBottom
        }
    }
}
