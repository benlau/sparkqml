// Spark's mockup can not use package name due to the conflict with current build binary
import QtQuick 2.0
import "../../../app/sparkqml/Spark/views/browserpanel"

Rectangle {
    width: 480
    height: 640
    color: "white"

    Grid {
        anchors.centerIn: parent
        spacing: 8

        ComponentCard {
            qml: "Item.qml"
            ui: "ItemForm.ui.qml"
        }

        ComponentCard {
            ui: "ItemForm.ui.qml"
        }

    }

}
