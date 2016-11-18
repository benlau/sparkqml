import QtQuick 2.0
import QtTest 1.0
import Shell 1.0
import Spark.js.redux 1.0
import Spark.js.qtredux 1.0
import Spark.js.immutabilityhelper 1.0
import Spark.actions 1.0

Item {

    TestCase {
        name: "Redux"

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

        function test_shallowDiff() {
            var obj = {}
            compare(QtRedux.shallowDiff(obj, obj), undefined);
            compare(QtRedux.shallowDiff({}, {}), {});
            compare(QtRedux.shallowDiff({v1: 1},{v1:1, v2:2}), {v2:2});

            var d1 = {
                o1: {
                    v1: 1
                }
            };
            var d2 = {
                o1: {
                    v1: 1,
                    v2: 2
                }
            }
            var res = {
                o1: {
                    v2: 2
                }
            }

            compare(QtRedux.shallowDiff(d1,d2), res);

            compare(QtRedux.shallowDiff([],[]), []);
            compare(QtRedux.shallowDiff([3,4],[1,2]), [1,2]);
        }

        function test_ImmutabilityHelper() {
            var state1 = ["x"];
            var state2 = ImmutabilityHelper.update(state1, {$push: ["y"]}); // ['x', 'y']
            compare(state2, ["x", "y"])
        }

        ReduxActionCreator {
            id: actions
        }

        function test_ActionCreator() {
            var store = Redux.createStore(function(state, action) {
                if (state === undefined) {
                    return { actions: [ action.type] };
                }
                state.actions.push(action.type);
                return state;
            });

            actions.dispatch = store.dispatch;
            actions.startApp();
            var state = store.getState();
            compare(state.actions.length, 2);
            compare(state.actions[1], "startApp");
        }

    }
}
