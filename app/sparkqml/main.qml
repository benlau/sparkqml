import QtQuick 2.3
import QtQuick.Window 2.2
import QtQuick.Controls 2.0
import Spark.constants 1.0
import Spark.adapters 1.0
import Spark.actions 1.0
import Spark.sys 1.0
import Spark.reducers 1.0
import Spark.js.redux 1.0
import Spark.js.qtredux 1.0
import Qt.labs.settings 1.0
import QtQuick.Dialogs 1.2
import "./Spark/views/mainpanel"
import "./Spark/views"
import "./Spark/views/navigationpanel"

ApplicationWindow {
    id: mainWindow
    visible: provider.mainWindowVisible
    width: 640
    height: 480
    title: provider.mainWindowTitle

    property var store

    Settings {
        id: mainSettings
        property alias x: mainWindow.x
        property alias y: mainWindow.y
        property alias width: mainWindow.width
        property alias height: mainWindow.height
        property string saveFolder: ""
    }

    ReduxActionCreator {
        id: actions
        objectName: "actions"
    }

    StoreProvider {
        id: provider
        objectName: "provider"
    }

    FileDialog {
        id: mainFileDialog
    }

    FileWatcher {
        source: provider.source
        onChanged: {
            actions.reload();
        }
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

    Component.onCompleted: {
        var middlewares = Redux.applyMiddleware(
                        CopyToFileMiddleware.create(mainFileDialog, mainSettings),
                        QuitMiddleware.create(),
                        QtRedux.createSignalProxyMiddleware(provider),
                        QtRedux.createSyncMiddleware(provider)
                    );
        mainWindow.store = Redux.createStore(App.reducer, middlewares);
        actions.dispatch = store.dispatch
    }

}
