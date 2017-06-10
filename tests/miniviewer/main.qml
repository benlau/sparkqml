import QtQuick 2.8
import QtQuick.Window 2.2
import Spark.views.components 1.0

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    ComponentViewer {
        id: viewer
        anchors.fill: parent
    }

    Component.onCompleted: {
        viewer.load(Qt.resolvedUrl("../sparkqmltests/mockup/mockup_NavigationPanel.qml"))
    }

}
