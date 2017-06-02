import QtQuick 2.0
import QtQuick 2.6
import QtQuick.Layouts 1.1
import QUIKit 1.0

Rectangle {
    id: component

    property alias folder : listModel.folder

    property alias filters: listModel.filters

    property alias options: listModel.options

    property alias spacing: flow.spacing

    height: flow.height

    color: "white"

    QmlFileListModel {
        id: listModel
    }

    BoxPackingFlow {
        id: flow
        width: parent.width
        model: listModel
        delegate: NameTag {
            name: model.qml
            growToFitContent: true

            onRefreshed: {
                flow.refresh();
            }

            ScaledLoader {
                autoScaleToFit: false
                source: model.source
                properties: model.properties
            }
        }
    }
}
