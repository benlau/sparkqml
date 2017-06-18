import QtQuick 2.4
import Spark.actions 1.0

Item {
    id: component
    width: 400
    height: 32

    property string name: "";

    property alias text: textItem.text

    property bool selected: provider.selectedState === name;

    MouseArea {
        id: mouseArea
        anchors.fill: parent

        onClicked: {
            actions.setSelectedState(name);
            component.forceActiveFocus();
        }
    }

    Text {
        id: textItem
        x: 16
        y: 0
        color: "#de000000"
        text: qsTr("State")
        verticalAlignment: Text.AlignVCenter
        anchors.leftMargin: 16
        horizontalAlignment: Text.AlignLeft
        anchors.fill: parent
        font.pixelSize: 14
    }

    Rectangle {
        id: mask
        anchors.fill: parent
        color: "white"
        visible: false
        opacity: 0.3
    }

    states: [
        State {
            name: "Selected"
            when: selected

            PropertyChanges {
                target: mask
                color: "#377fd6"
                visible: true
            }
        }
    ]
}
