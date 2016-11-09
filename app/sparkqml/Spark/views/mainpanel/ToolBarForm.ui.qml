import QtQuick 2.4
import FontAwesome 1.0
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3

Rectangle {
    width: 300
    height: 24
    color: "#313334"
    property alias saveButton: saveButton
    property alias clipboardButton: clipboardButton
    property alias scaleToFitButton: scaleToFitButton
    property alias barButton: barButton
    property alias reloadButton: reloadButton

    RowLayout {
        id: layout
        spacing: 0
        anchors.fill: parent

        ToolBarButton {
            id: barButton
            height: 24
            Layout.fillHeight: true
            icon: FontAwesome.bars
        }

        Item {
            id: spacer
            Layout.fillHeight: true
            Layout.fillWidth: true
        }

        ToolBarButton {
            id: saveButton
            icon: FontAwesome.fileO
            toolTip: qsTr("Save")
        }

        ToolBarButton {
            id: clipboardButton
            x: 119
            y: 8
            icon: FontAwesome.clipboard
            toolTip: qsTr("Copy to Clpboard")
        }

        ToolBarButton {
            id: scaleToFitButton
            toolTip: qsTr("Scale to Fit")
            icon: FontAwesome.arrowsAlt
            Layout.fillWidth: false
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
        }

        ToolBarButton {
            id: reloadButton
            toolTip: qsTr("Reload")
            Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
            Layout.minimumHeight: 0
            icon: FontAwesome.refresh
        }




    }
}
