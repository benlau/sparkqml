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
    }
}
