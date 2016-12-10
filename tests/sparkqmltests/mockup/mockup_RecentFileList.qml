// Spark's mockup can not use package name due to the conflict with current build binary
import QtQuick 2.0
import "../../../app/sparkqml/Spark/views/navigationpanel"

Rectangle {
    id: rectangle1
    width: 160
    height: 28 * 4

    color: "#e7e7e7"

    property var model1: [
        {
            file: "Test1.qml",
            basename: "Test1.qml"
        },
        {
            file: "Test2.qml",
            basename: "Test2.qml"
        },
        {
            file: "Test3.qml",
            basename: "Test3.qml"
        }
    ]

    property var model2: []

    RecentFileList {
        id: recentFileList
        anchors.fill: parent
        model: model1
    }
    states: [
        State {
            name: "3.5 Files"

            PropertyChanges {
                target: rectangle1
                height: 98
            }
        },
        State {
            name: "No Files"
            PropertyChanges {
                target: rectangle1
                height: 98
            }

            PropertyChanges {
                target: recentFileList
                model: model2
            }
        }
    ]

}
