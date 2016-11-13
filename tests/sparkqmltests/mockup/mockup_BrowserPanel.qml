// Spark's mockup can not use package name due to the conflict with current build binary
import QtQuick 2.0
import "../../../app/sparkqml/Spark/views/browserpanel"

Item {
    width: 480
    height: 640

    ListModel {
        id: gridModel

        ListElement {
            qml: "Component.qml"
            ui: "ComponentForm.ui.qml"
        }

        ListElement {
            qml: "Component.qml"
            ui: "ComponentForm.ui.qml"
        }

    }

    BrowserPanel {
        anchors.fill: parent
        model: gridModel
    }

}
