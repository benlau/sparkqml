import QtQuick 2.7
import QtQuick.Window 2.2
import QtQuick.Controls 2.0
import Spark.constants 1.0
import Spark.actions 1.0
import Spark.sys 1.0
import Spark.reducers 1.0
import Qt.labs.settings 1.0
import QtQuick.Dialogs 1.2
import Spark.store 1.0
import QRedux 1.0
import "./Spark/views/mainpanel"
import "./Spark/views"
import "./Spark/views/navigationpanel"

ApplicationWindow {
    id: mainWindow
    visible: provider.mainWindowVisible
    width: 640
    height: 480
    title: provider.mainWindowTitle

    Settings {
        category: "Window"
        property alias x: mainWindow.x
        property alias y: mainWindow.y
        property alias width: mainWindow.width
        property alias height: mainWindow.height
    }

    FocusScope {
        anchors.fill: parent

        ViewStack {
            anchors.fill: parent
        }

        NavigationDrawer {
            id: drawer
            height: mainWindow.height
        }
    }
}
