import QtQuick 2.0
import QUIKit 1.0

Item {
    width: 640
    height: 480
    clip: true

    UIGrid {
        folder: Qt.resolvedUrl("../sample/rectanlges/");
        anchors.fill: parent
    }

}
