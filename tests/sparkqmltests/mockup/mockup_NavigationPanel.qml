// Spark's mockup can not use package name due to the conflict with current build binary
import QtQuick 2.0
import "../../../app/sparkqml/Spark/views/browserpanel"
import "../../../app/sparkqml/Spark/views/navigationpanel"
import "../../../app/sparkqml/Spark/reducers"

Rectangle {
    width: 480
    height: 640
    color: "white"

    StoreProvider {
        id: provider

        recentFiles: ([
            {basename: "Test1.qml"},
            {basename: "SuperLongFileName.qml"}
        ])
    }

    NavigationPanel {
        height: parent.height

    }

}
