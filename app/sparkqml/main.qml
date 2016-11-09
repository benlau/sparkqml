import QtQuick 2.3
import QtQuick.Window 2.2
import QtMultimedia 5.5
import QtQuick.Controls 2.0
import Spark.constants 1.0
import Spark.adapters 1.0
import Spark.actions 1.0
import Spark.stores 1.0
import Spark.sys 1.0
import Qt.labs.settings 1.0
import QtQuick.Dialogs 1.2
import "./Spark/views"

ApplicationWindow {
    id: mainWindow
    visible: false
    width: 640
    height: 480
    title: MainStore.title

    Settings {
        id: mainSettings
        property alias x: mainWindow.x
        property alias y: mainWindow.y
        property alias width: mainWindow.width
        property alias height: mainWindow.height
        property string saveFolder: ""
    }

    QtObject {
        id: mainContext

        property var mainViewer: mainPanel.mainViewer
    }

    FileDialog {
        id: mainFileDialog
    }

    StoreAdapter {
    }

    FileWatcher {
        source: MainStore.source
        onChanged: {
            AppActions.reload();
        }
    }

    MainPanel {
        id: mainPanel
        anchors.fill: parent
    }

    Drawer {
        id: mainDrawer
        dim: true
        dragMargin: 0
        width: 160
        height: mainWindow.height

        NavigationPanel {
            anchors.fill: parent
        }
    }

}
