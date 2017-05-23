import QtQuick 2.0
import QtQuick 2.6
import QtQuick.Layouts 1.1
import QUIKit 1.0

Rectangle {
    id: component

    property alias folder : listModel.folder

    property alias filters: listModel.filters

    height: grid.height
    width: grid.width + grid.leftPadding + grid.rightPadding

    color: "white"

    QmlFileListModel {
        id: listModel
    }

    Flow {
        id: grid
        spacing: 10
        padding: 10
        anchors.centerIn: parent

        Repeater {
            model: listModel

            delegate: NameTag {
                name: model.qml
                growToFitContent: true

                ScaledLoader {
                    autoScaleToFit: false
                    source: model.source
                }
            }
        }
    }
}
