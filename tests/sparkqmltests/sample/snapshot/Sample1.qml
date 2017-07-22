import QtQuick 2.0

Item {
    id: sample1
    x: 10
    y: 10
    width: 640
    height: 480

    Item {
        id: item1
        anchors.fill: parent
    }

    Item {
        id: item2
        width: 100
        height: 100
        anchors.right: parent.right
    }
}
