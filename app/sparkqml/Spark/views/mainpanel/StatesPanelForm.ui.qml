import QtQuick 2.4
import QtQuick.Layouts 1.1

Rectangle {
    id: item1
    width: 400
    height: 400
    color: "#e7e7e7"
    property alias columnLayout1: columnLayout1
    property alias listView: listView


    ColumnLayout {
        id: columnLayout1
        spacing: 0
        anchors.fill: parent

        Rectangle {
            id: header
            x: 161
            y: 192
            width: 200
            height: 200
            color: "#fefefe"
            Layout.maximumHeight: 24
            Layout.fillHeight: true
            Layout.fillWidth: true

            Text {
                id: text1
                color: "#de000000"
                text: qsTr("States")
                verticalAlignment: Text.AlignVCenter
                anchors.leftMargin: 8
                anchors.topMargin: 0
                anchors.fill: parent
                font.pixelSize: 14
            }
        }

        Rectangle {
            id: content
            height: 16
            color: "#fafafa"
            Layout.fillHeight: true
            Layout.fillWidth: true

            StateListView {
                id: listView
                anchors.topMargin: 0
                spacing: 0
                anchors.fill: parent
                delegate: StateItem {
                    text: modelData.displayName
                    name: modelData.name
                }
            }
        }

    }
}
