import QtQuick 2.4
import QtQuick.Controls 2.0

Rectangle {
    id: component
    width: 500
    height: 300
    color: "#FFFDBE"
    property string message: qsTr("Error Message")

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

    Button {
        id: button1
        x: 368
        y: 225
        text: qsTr("OK")
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 16
        anchors.right: parent.right
        anchors.rightMargin: 32
    }

    Text {
        id: errorMessageText
        text: component.message
        anchors.bottomMargin: 68
        anchors.rightMargin: 32
        anchors.leftMargin: 32
        anchors.topMargin: 68
        anchors.fill: parent
        font.pixelSize: 12
    }
}
