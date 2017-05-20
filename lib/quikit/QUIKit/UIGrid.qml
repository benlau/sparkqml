import QtQuick 2.0
import QtQuick 2.4
import QtQuick.Layouts 1.1
import QUIKit 1.0

Rectangle {
    id: component

    property alias cellWidth: grid.cellWidth
    property alias cellHeight: grid.cellHeight

    property alias folder : listModel.folder

    color: "white"

    QmlFileListModel {
        id: listModel
    }

    GridView {
        id: grid
        interactive: false
        anchors.fill: parent
        model: listModel
        cellWidth: 200
        cellHeight: 150

        delegate: Item {
            width: grid.cellWidth
            height: grid.cellHeight

            NameTag {
                name: model.qml
                anchors.fill: parent
                anchors.margins: 4

                ScaledLoader {
                    source: model.source
                }
            }
        }

    }
}
