import QtQuick 2.0
import "../../../lib/quikit/QUIKit/"

Item {
    width: 640
    height: 480
    clip: true

    ScaledContentFrame {
        width: 700
        height: 600

        UIFlow {
            folder: Qt.resolvedUrl("../sample/rectanlges/");
        }
    }

}
