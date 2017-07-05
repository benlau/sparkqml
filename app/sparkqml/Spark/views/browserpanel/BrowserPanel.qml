import QtQuick 2.4
import QtQuick.Layouts 1.3
import Spark.sys 1.0
import QUIKit 1.0
import Future 1.0

Item {
    implicitWidth: 640
    implicitHeight: 480

    property alias model: componentGridView.model

    function browse(folder) {
        var future = Engine.scanImportPathList(folder);
        Future.onFinished(future, function() {
            listModel.folder = folder;
        });
    }

    ColumnLayout {
        id: columnLayout1
        spacing: 0
        anchors.fill: parent

        BrowserActionBar {
            z: 1
            Layout.alignment: Qt.AlignLeft | Qt.AlignTop
            Layout.fillWidth: true
        }

        Rectangle {
            color: "#eaeaea"
            clip: true
            Layout.fillWidth: true
            Layout.fillHeight: true

            ComponentGridView {
                id: componentGridView

                model: QmlFileListModel {
                    id: listModel
                }

                anchors.fill: parent
                anchors.margins: 10
            }
        }

    }

    Connections {
        target: provider
        ignoreUnknownSignals: true
        onBrowsingFolderChanged: {
            browse(provider.browsingFolder);
        }
    }

    Component.onCompleted: {
        browse(provider.browsingFolder);
    }
}



