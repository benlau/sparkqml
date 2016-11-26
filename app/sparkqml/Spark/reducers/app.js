.pragma library
.import "../js/immutabilityhelper/immutabilityhelperwrapper.js" as ImmutabilityHelper
.import Shell 1.0 as ShellWrapper
.import Spark.sys 1.0 as SparkSys
.import "../js/lodash/lodashwrapper.js" as Lodash

var update = ImmutabilityHelper;
var Shell = ShellWrapper.Shell;
var Url = SparkSys.Url;

var initState = {
    mainWindowVisible: false,
    mainWindowTitle: "Spark",
    source: "",
    fileName: "",
    folder: "",
    selectedState: "",
    availableStates: []
}

function reducer(state, actions) {
    if (state === undefined) {
        return initState;
    }

    var ops = {};

    switch (actions.type) {
    case "startApp":
        ops = {
            "mainWindowVisible": { $set: true}
        };

        state = update(state, ops);
        break;

    case "load":
        var path = Url.path(actions.source);
        ops = {
            source: {$set: actions.source},
            fileName: {$set: Shell.basename(path)},
            folder: {$set: Shell.dirname(path)},
            mainWindowTitle: {$set: Shell.basename(path)}
        };
        state = update(state, ops);
        break;

    case "setSelectedState":
        ops = {
            selectedState: {$set: actions.state}
        }
        state = update(state, ops);
        break;

    case "moveSelectedState":
        if (state.availableStates.length === 0) {
            return state;
        }

        var index = Lodash.findIndex(state.availableStates, function(item) { return item.name === state.selectedState});
        if (index < 0) {
            index = 0;
        } else if (actions.direction === "up") {
            index--;
        } else {
            index++;
        }

        if (index < 0) {
            index = 0;
        } else if (index >= state.availableStates.length) {
            index = state.availableStates.length - 1;
        }

        var selectedState = state.availableStates[index].name;
        if (state.selectedState !== selectedState) {
            state = Lodash.assign({}, state, {selectedState: selectedState});
        }
        break;
    }

    return state;
}
