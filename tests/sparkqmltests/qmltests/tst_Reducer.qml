import QtQuick 2.0
import QtTest 1.0
import Shell 1.0
import Spark.js.redux 1.0
import Spark.js.qtredux 1.0
import Spark.js.immutabilityhelper 1.0
import Spark.actions 1.0
import Spark.reducers 1.0

Item {

    TestCase {
        name: "Reducer"

        ActionCreator {
            id: actions;
        }

        function test_startApp() {
            var store = Redux.createStore(App.reducer);
            actions.dispatch = store.dispatch;
            var state = store.getState();
            compare(state.mainWindowVisible, false);

            actions.startApp();

            var newState = store.getState();
            compare(state !== newState, true);
            compare(newState.mainWindowVisible, true);
        }

        function test_load() {
            var store = Redux.createStore(App.reducer);
            var state = store.getState();
            actions.dispatch = store.dispatch;

            state.errorString = "Error";

            actions.load("file:///folder/App.qml");
            state = store.getState();
            compare(state.source, "file:///folder/App.qml");
            compare(state.fileName, "App.qml");
            compare(state.folder, "/folder");
            compare(state.mainWindowTitle, "App.qml");
            compare(state.selectedState, "");
            compare(state.errorString, "");
        }

        function test_moveSelectedState() {
            var store = Redux.createStore(App.reducer);
            actions.dispatch = store.dispatch;
            var state = store.getState();

            compare(state.selectedState, "");

            actions.moveSelectedState("up");
            actions.moveSelectedState("down");

            state.availableStates = [ {name: ""},
                                      {name: "S1"},
                                      {name: "S2"},
                                      {name: "S3"}];

            actions.moveSelectedState("down");

            state = store.getState();
            compare(state.selectedState, "S1");

        }

        function test_addRecentFiles() {
            var store = Redux.createStore(App.reducer);
            actions.dispatch = store.dispatch;

            function convert(input) {
                return input.map(function(item) {
                    return {
                        file: item,
                        displayName: item
                    }
                });
            }

            for (var i = 0 ;i < 10;i++) {
                actions.addRecentFile(i.toString());
            }

            compare(store.getState().recentFiles, convert(["9","8","7","6","5","4","3","2","1","0"]));
            actions.addRecentFile("11");
            compare(store.getState().recentFiles, convert(["11","9","8","7","6","5","4","3","2","1"]));

            actions.addRecentFile("3");
            compare(store.getState().recentFiles, convert(["3","11","9","8","7","6","5","4","2","1"]));
        }
    }
}
