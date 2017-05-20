import QtQuick 2.0
import QtQuick 2.4
import QtQuick.Layouts 1.1
import QUIKit 1.0

Rectangle {
    id: component

    color: "white"

    property alias cellWidth: grid.cellWidth
    property alias cellHeight: grid.cellHeight

    property alias folder : listModel.folder

    QmlFileListModel {
        id: listModel
    }

    GridView {
        id: grid
        anchors.fill: parent
        model: listModel
        cellWidth: 200
        cellHeight: 150

        delegate: Item {
            width: grid.cellWidth
            height: grid.cellHeight

            ColumnLayout  {
                anchors.fill: parent
                anchors.margins: 4

                ScaledLoader {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    source: model.source
                }

                Text {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    Layout.maximumHeight: 20
                    text: model.qml
                    color: "#de000000"
                    horizontalAlignment: Text.AlignHCenter
                    font.pixelSize: 14
                    elide: Text.ElideMiddle
                    verticalAlignment: Text.AlignVCenter
                }
            }
        }

    }
}
