import QtQuick 2.8
import QtQuick.Window 2.2
import Spark.views.components 1.0
import Spark.views.mainpanel 1.0
import QRedux 1.0
import Spark.reducers 1.0
import Spark.actions 1.0
import Spark.store 1.0

Window {
    id: mainWindow
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    property var store: null

    MainPanel {
        anchors.fill: parent
    }

//    MainComponentViewer {
//        id: viewer
//        anchors.fill: parent
//    }

    ActionCreator {
        id: actions
        objectName: "actions"
    }

    StoreProvider {
        id: provider
        objectName: "provider"
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
                        logger,
                        SystemMiddleware.create(provider),
                        LoadingMiddleware.create(),
                        QRedux.signalProxyMiddleware(provider),
                        QRedux.syncMiddleware(provider)
                    );
        mainWindow.store = Redux.createStore(App.reducer, middlewares);
        actions.dispatch = store.dispatch

        actions.load(Qt.resolvedUrl("../sparkqmltests/mockup/mockup_NavigationPanel.qml"));
    }

}
