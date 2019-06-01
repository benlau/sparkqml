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
import Qt.labs.platform 1.1 as LabsPlatform
import QRedux 1.0
import "./Spark/views/mainpanel"
import "./Spark/views"
import "./Spark/views/navigationpanel"

Object {
    id: component

    property var store

    Settings {
        id: mainSettings
        property string saveFolder: ""
        property string recentFiles: ""
    }

    ActionCreator {
        id: actions
        objectName: "actions"
    }

    StoreProvider {
        id: provider
        objectName: "provider"
    }

    FileWatcher {
        source: provider.source
        onChanged: {
            actions.reload();
        }
    }

    QtObject {
        id: mainContext

        property var mainFileDialog
    }

    Loader {
        asynchronous: true

        sourceComponent: MainWindow {
            id: mainWindow

            visible: provider.mainWindowVisible

            LabsPlatform.FileDialog {
                id: mainFileDialog
                fileMode: LabsPlatform.FileDialog.SaveFile
            }

            Component.onCompleted: {
                mainContext.mainFileDialog = mainFileDialog;
            }
        }
    }

    Component.onCompleted: {
        function logger(store) {
            return function (next) {
                return function(action) {
                    console.log(action.type);
                    next(action);
                }
            }
        }

        var middlewares = QRedux.applyMiddleware(
//                        logger,
                        LoadingMiddleware.create(),
                        CopyToFileMiddleware.create(mainContext, mainSettings),
                        SystemMiddleware.create(provider),
                        SettingsMiddleware.create(mainSettings),
                        QRedux.signalProxyMiddleware(provider),
                        QRedux.syncMiddleware(provider)
                    );
        component.store = Redux.createStore(App.reducer, middlewares);
        actions.dispatch = store.dispatch
    }

}
