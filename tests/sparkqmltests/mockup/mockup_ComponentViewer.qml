import QtQuick 2.0
import Spark.views.components 1.0

Rectangle {
    width: 480
    height: 640
    color: "black"

    ComponentViewer {
        id: viewer
        anchors.fill: parent
        source: Qt.resolvedUrl("../sample/Rect.qml")
    }

    states: [
        State {
            name: "scaleToFit"

            StateChangeScript {
                script: viewer.scaleToFit()
            }
        },
        State {
            name: "resizeToFit"

            StateChangeScript {
                script: viewer.resizeToFit()
            }

        }
    ]

}
