import QtQuick 2.4
import QtQuick.Layouts 1.3
import Spark.sys 1.0
import Spark.stores 1.0
import Future 1.0

BrowserPanelForm {

    function browse(folder) {
        var future = Engine.scanImportPathList(folder);
        Future.onFinished(future, function() {
            listModel.folder = folder;
        });
    }

    model: QmlFileListModel {
        id: listModel
    }

    Component.onCompleted: {
        browse(MainStore.browsingFolder);
    }
}
