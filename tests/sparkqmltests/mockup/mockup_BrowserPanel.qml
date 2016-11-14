// Spark's mockup can not use package name due to the conflict with current build binary
import QtQuick 2.0
import "../../../app/sparkqml/Spark/views/browserpanel"

Item {
    width: 480
    height: 640

    ListModel {
        id: gridModel
    }

    BrowserPanel {
        anchors.fill: parent
        model: gridModel
    }

    Component.onCompleted: {
        for (var i = 0 ; i < 10;i++) {
            gridModel.append({
                 source: Qt.resolvedUrl("../sample/Rect.qml"),
                 preview: Qt.resolvedUrl("../sample/Rect.qml"),
                 qml: "Component.qml",
                 ui: "ComponentForm.ui.qml"
            });
        }
    }

}
