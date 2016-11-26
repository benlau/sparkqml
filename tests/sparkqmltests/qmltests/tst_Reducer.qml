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

    TestCase {
        name: "Reducer"

        ReduxActionCreator {
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
            actions.dispatch = store.dispatch;

            actions.load("file:///folder/App.qml");
            var state = store.getState();
            compare(state.source, "file:///folder/App.qml");
            compare(state.fileName, "App.qml");
            compare(state.folder, "/folder");
            compare(state.mainWindowTitle, "App.qml");
            compare(state.selectedState, "");
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
    }
}
