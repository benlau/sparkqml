import QtQuick 2.0
import "../../../lib/quikit/QUIKit/"

Item {
    width: 640
    height: 480
    clip: true

    UIFlow {
        folder: Qt.resolvedUrl("../sample/rectanlges/");
        anchors.fill: parent
    }

}
