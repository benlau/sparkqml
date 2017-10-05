import QtQuick 2.7
import FontAwesome 1.0
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3

Rectangle {
    width: 300
    height: 24
    color: "#313334"
    property alias resizeButton: resizeButton
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
            iconSource: FontAwesome.bars
            onClicked: actions.openDrawer();
        }

        Item {
            id: spacer
            Layout.fillHeight: true
            Layout.fillWidth: true
        }

        ToolBarButton {
            id: saveButton
            iconSource: FontAwesome.fileO
            toolTip: qsTr("Save")
            onClicked: {
                actions.askToSaveFile();
            }
        }

        ToolBarButton {
            id: clipboardButton
            x: 119
            y: 8
            iconSource: FontAwesome.clipboard
            toolTip: qsTr("Copy to Clpboard")
            onClicked: {
                actions.copyToClipboard();
            }
        }

        ToolBarButton {
            id: resizeButton
            iconSource: FontAwesome.windowMaximize
            toolTip: qsTr("Resize to Fit")
            onClicked: {
                actions.resizeToFit();
            }
        }

        ToolBarButton {
            id: scaleToFitButton
            toolTip: qsTr("Scale to Fit")
            iconSource: FontAwesome.arrowsAlt
            Layout.fillWidth: false
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
            onClicked: {
                onClicked: actions.scaleToFit();
            }
        }

        ToolBarButton {
            id: reloadButton
            toolTip: qsTr("Reload")
            Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
            Layout.minimumHeight: 0
            iconSource: FontAwesome.refresh
            onClicked: actions.reload();
        }
    }

    Shortcut {
        sequence: "Ctrl+R"
        onActivated: actions.reload();
    }
}
