import QtQuick 2.0
import QtTest 1.0
import Shell 1.0
import Spark.js.redux 1.0
import Spark.js.qtredux 1.0
import Spark.js.immutabilityhelper 1.0
import Spark.actions 1.0
import Spark.reducers 1.0
import Spark.actions 1.0

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

    TestCase {
        name: "Middleware"

        Item {
            id: mockDialog
            property bool selectMultiple
            property bool selectExisting
            property var nameFilters
            property string folder

            function open() {
            }
        }

        Item {
            id: mockSettings
            property string saveFolder: "";
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
    }
}
