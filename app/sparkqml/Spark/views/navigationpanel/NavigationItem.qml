import QtQuick 2.4
import QtQuick.Controls 2.0
import FontAwesome 1.0

MouseArea {
    id: component
    height: 28

    property alias icon: text1.text

    property alias text: text2.text
    
    Text {
        id: text1
        y: 5
        text: FontAwesome.signOut
        anchors.left: parent.left
        anchors.leftMargin: 8
        anchors.verticalCenter: parent.verticalCenter
        font.pixelSize: 14
        font.family: FontAwesome.fontFamily
    }
    
    Text {
        id: text2
        height: 24
        text: qsTr("")
        anchors.left: parent.left
        anchors.leftMargin: 28
        anchors.right: parent.right
        anchors.rightMargin: 8
        anchors.verticalCenter: parent.verticalCenter
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignVCenter
        font.pixelSize: 14
        elide: Text.ElideMiddle
    }
    
    Rectangle {
        anchors.fill: parent
        color: "#3FFFFFFF"
        visible: component.pressed
    }
    
}
