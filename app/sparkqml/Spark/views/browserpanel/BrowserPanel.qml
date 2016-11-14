import QtQuick 2.4
import QtQuick.Layouts 1.3
import Spark.sys 1.0
import Spark.stores 1.0

BrowserPanelForm {
    model: QmlFileListModel {
        folder: MainStore.browsingFolder
    }
}
