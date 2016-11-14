// Spark's mockup can not use package name due to the conflict with current build binary
import QtQuick 2.0
import "../../../app/sparkqml/Spark/views"

Item {
    width: 640
    height: 480

    ViewStack {
        id: viewStack
        anchors.fill: parent
    }

    states: [
        State {
            name: "BrowserPanel"

            PropertyChanges {
                target: viewStack
                views: [
                    {
                        name: "browserPanel"
                    }
                ]
            }
        }
    ]
}
