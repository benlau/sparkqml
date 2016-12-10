import QtQuick 2.0
import FontAwesome 1.0

Column {
    id: component
    width: 160

    property alias model: repeater.model

    Item {
        width: parent.height
        height: 28

        Text {
            id: text2
            x: 8
            y: 182
            height: 24
            text: qsTr("RECENT FILES")
            anchors.left: parent.left
            anchors.leftMargin: 8
            anchors.verticalCenter: parent.verticalCenter
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            wrapMode: Text.WordWrap
            font.pixelSize: 12
            color: "#9C000000"
        }
    }

    Repeater {
        id: repeater

        delegate: RecentFileItem {
            width: component.width
            displayName: modelData.displayName
            file: modelData.file
            visible: (index+2) * 28 <= component.height
            enabled: visible
        }
    }

}
