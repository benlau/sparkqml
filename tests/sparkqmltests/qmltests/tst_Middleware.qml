import QtQuick 2.0
import QtTest 1.0
import QUIKit 1.0
import Spark.actions 1.0
import Spark.reducers 1.0

Item {

    function logger(log) {
        return function () {
            return function(next) {
                return function(action) {
                    log.push(action.type);
                    next(action);
                }
            }
        }
    }

    function defaultReducer(state) {
        if (state === undefined) {
            return {};
        }
        return state;
    }

    TestCase {
        name: "Middleware"

        Item {
            id: mockDialog
            property bool selectMultiple
            property bool selectExisting
            property var nameFilters
            property string folder

            signal accepted
            signal rejected

            function open() {
            }
        }

        Item {
            id: mockSettings
            property string saveFolder: "";
            property var recentFiles: []
        }

        function test_CopyToFileMiddleware() {
            var log = [];
            function reducer(state, action) {
                if (state === undefined) {
                    return {
                        source: "file:///Main.qml",
                        fileName: "Main.qml",
                        selectedState: "Test"
                    }
                }
                return state;
            }
            var middlewares = Redux.applyMiddleware(logger(log),
                                                    CopyToFileMiddleware.create(mockDialog, mockSettings));
            var store = Redux.createStore(reducer,middlewares);

            store.dispatch({type: "askToSaveFile"});
            compare(log.length, 1);
            compare(mockDialog.folder, "file://Main(Test).png");
        }

        function test_settingsmiddleware() {
            var log = [];

            var middlewares = [
                SettingsMiddleware.create(mockSettings),
                logger(log)
            ];
            var store = Redux.createStore(App.reducer, Redux.applyMiddleware.apply(this, middlewares));
            compare(store.getState().recentFiles, []);

            store.dispatch({type: "startApp"})
            compare(log.length, 2);
            compare(log[0], "setRecentFiles");
            compare(log[1], "startApp");
        }
    }
}
