import QtQuick 2.0
import QtQuick.Layouts 1.3

Item {

    id: component

    property alias model: gridView.model

    GridView {
        id: gridView
        anchors.fill: parent
        cellWidth: 200
        cellHeight: 200

        delegate: ComponentCard {

        }
    }

}
