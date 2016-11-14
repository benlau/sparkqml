import QtQuick 2.0
import QtQuick.Layouts 1.3

Item {

    id: component

    property alias model: gridView.model

    property int expectedCellWidth: 200
    property int expectedCellHeight: 200

    GridView {
        id: gridView
        anchors.fill: parent

        cellWidth: component.width / Math.floor(component.width / component.expectedCellWidth);
        cellHeight: component.expectedCellHeight

        delegate: Item {
            width: gridView.cellWidth
            height: gridView.cellHeight
            ComponentCard {
                source: model.source
                preview: model.preview
                ui: model.ui
                qml: model.qml
                anchors.centerIn: parent
            }
        }
    }

}
