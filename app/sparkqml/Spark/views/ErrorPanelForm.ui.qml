import QtQuick 2.4
import QtQuick.Controls 2.0

Rectangle {
    id: component
    width: 500
    height: 300
    color: "#FFFDBE"
    property alias reloadButton: reloadButton
    property alias confirmButton: confirmButton
    property string message: qsTr("Error Message")

    MouseArea {
        id: mouseArea1
        anchors.fill: parent
    }

    Text {
        id: text1
        text: qsTr("Cannot open this QML document because of an error in the QML file:")
        wrapMode: Text.WordWrap
        anchors.right: parent.right
        anchors.rightMargin: 32
        anchors.left: parent.left
        anchors.leftMargin: 16
        anchors.top: parent.top
        anchors.topMargin: 16
        font.bold: true
        style: Text.Normal
        font.pixelSize: 14
        elide: Text.ElideRight
    }

    Text {
        id: errorMessageText
        text: component.message
        wrapMode: Text.WordWrap
        anchors.bottomMargin: 68
        anchors.rightMargin: 32
        anchors.leftMargin: 32
        anchors.topMargin: 68
        anchors.fill: parent
        font.pixelSize: 12
        clip: true
    }

    Row {
        id: row
        height: 40
        spacing: 16
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 16
        anchors.right: parent.right
        anchors.rightMargin: 32

        Button {
            id: confirmButton
            text: qsTr("Close")
        }

        Button {
            id: reloadButton
            text: qsTr("Reload")
        }
    }

}
