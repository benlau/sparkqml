import QtQuick 2.0
import QtTest 1.0
import Shell 1.0
import Spark.js.redux 1.0
import Spark.js.qtredux 1.0
import Spark.js.immutabilityhelper 1.0
import Spark.actions 1.0
import Spark.js.lodash 4.17

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

        function test_Middleware() {
            var reducer = function(state, action) {
                if (state === undefined) {
                    return {value: 0}
                }
                if (action.hasOwnProperty("value")) {
                    return {
                        value: action.value + state.value
                    }
                }
                return state;
            }
            var middleware = function(store) {
                return function(next) {
                    return function(action) {
                        if (action.hasOwnProperty("value")) {
                            action.value++;
                        }
                        next(action);
                    }
                }
            }

            var store = Redux.createStore(reducer, Redux.applyMiddleware(middleware));

            compare(store.getState().value, 0);
            store.dispatch({type:"dummy", value:1});
            compare(store.getState().value, 2);

        }

        Item {
            id: signalProxy

            signal signal1
            property int signal1Count: 0

            signal signal2(int v1, string v2)
            property int signal2Count: 0

            onSignal1: {
                signal1Count++;
            }

            onSignal2: {
                signal2Count++;
            }
        }

        function test_SignalProxyMiddleware() {
            var reducer = function() {};
            var store = Redux.createStore(reducer,
                                          Redux.applyMiddleware(QtRedux.createSignalProxyMiddleware(signalProxy)));

            store.dispatch({type: "signal1"});
            compare(signalProxy.signal1Count, 1);

            try { // Uncaught exception: Insufficient arguments
                store.dispatch({type: "signal2"});
            } catch (e) {
            }

            compare(signalProxy.signal2Count, 0);

            store.dispatch({type: "signal2", arguments: [1,"2"] });
            compare(signalProxy.signal2Count, 1);
        }

        function test_lodash() {
            var obj = Lodash.assign({}, {value: 1});
            compare(obj.value, 1);
        }

        Item {
            id: mockProvider
            property int value1: 0
            property real value2: 0.0
            property string value3: ""

            property alias value4 : subItem

            Item {
                id: subItem
                property int value1
            }

            property var value5
        }

        function test_assign() {
            QtRedux.assign(mockProvider, undefined);
            compare(mockProvider.value1, 0);

            QtRedux.assign(mockProvider, {});
            compare(mockProvider.value1, 0);

            QtRedux.assign(mockProvider, {
                               value0: 123,
                               value1: 1,
                               value2: 2.0,
                               value3: "3",
                               value4: {
                                   value1: 5
                               },
                               value5: ["abc", "def"]
                           });
            compare(mockProvider.value1, 1);
            compare(mockProvider.value2, 2.0);
            compare(mockProvider.value3, "3");
            compare(mockProvider.value4.value1, 5);
            compare(mockProvider.value5, ["abc", "def"]);
        }


        function test_SyncMiddleware() {
            var reducer = function(state, action) {
                if (state === undefined) {
                    return{
                        value1 :0
                    }
                }

                if (action.type === "inc") {
                    state = Lodash.assign({}, state, {value1: state.value1+1});
                }

                return state;
            };

            mockProvider.value1 = 0;

            var store = Redux.createStore(reducer,
                                          Redux.applyMiddleware(QtRedux.createSyncMiddleware(mockProvider)));

            store.dispatch({type: "inc"});
            compare(store.getState().value1, 1);
            compare(mockProvider.value1, 1);

            store.dispatch({type: "inc"});
            compare(store.getState().value1, 2);
            compare(mockProvider.value1, 2);
        }


    }
}
