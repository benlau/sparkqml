import QtQuick 2.0
import "../../../lib/quikit/QUIKit/"

Item {
    width: 640
    height: 480
    clip: true

    ScaledContentFrame {
        anchors.fill: parent

        UIBoxPackingFlow {
            width: 300
            folder: Qt.resolvedUrl("../sample/rectanlges/");
        }
    }

}
