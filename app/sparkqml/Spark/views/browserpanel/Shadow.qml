// Create Shadow on a rectangle object with simple parameter like box-shadow in css.
import QtQuick 2.0
import QtGraphicalEffects 1.0

Item {
    id : shadow

    property bool inverted: false

    Rectangle {
        id: dark
        anchors.fill: parent
        color: "black"
        visible: false;
    }

    DropShadow {
        source: dark
        anchors.fill: parent
        verticalOffset: inverted ? -1 : 1
        opacity: 0.2
        spread: 0.5
    }
}
