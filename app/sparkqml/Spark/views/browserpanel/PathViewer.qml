import QtQuick 2.0

Item {
    id: component

    height: 36

    property var path : new Array

    Row {
        anchors.fill: parent

        Repeater {
            model: path

            delegate: PathButton {
                height: 24
                text: modelData
                last: index === path.length - 1
            }
        }

    }

}
