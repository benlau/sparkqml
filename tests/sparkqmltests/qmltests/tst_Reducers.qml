import QtQuick 2.0
import QtTest 1.0
import Shell 1.0
import Spark.js.redux 1.0

Item {

    TestCase {
        name: "Reducers"

        function test_store_create() {
            var store = Redux.createStore(function(state, action) {
                if (state === undefined) {
                    return { actions: [ action.type] };
                }
                state.actions.push(action.type);
                return state;
            });
            var state = store.getState();
            compare(state.hasOwnProperty("actions"), true);
            compare(state.actions.length, 1)
            store.dispatch({type: "dummy"});
            compare(state.actions.length, 2);
            compare(state.actions[1], "dummy");
        }

    }
}
