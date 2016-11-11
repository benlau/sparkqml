// Spark's mockup can not use package name due to the conflict with current build binary
import QtQuick 2.0
import "../../../app/sparkqml/Spark/views/components"

Rectangle {
    width: 480
    height: 640
    color: "black"

    ComponentViewer {
        id: viewer
        anchors.fill: parent
    }

    states: [
        State {
            name: "load"
            StateChangeScript {
                script: viewer.load(Qt.resolvedUrl("../sample/Rect.qml"));
            }
        },

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

        },
        State {
            name: "reload"

            StateChangeScript {
                script: viewer.reload()
            }
        }

    ]

}
