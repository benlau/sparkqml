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
    errorString: "",
    browsingFolder: "",
    views: [],
    availableStates: []
}

function moveSelectedState(state, action) {
    if (state.availableStates.length === 0) {
        return state;
    }

    var index = Lodash.findIndex(state.availableStates, function(item) { return item.name === state.selectedState});
    if (index < 0) {
        index = 0;
    } else if (action.direction === "up") {
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

    return state;
}

function reducer(state, action) {
    if (state === undefined) {
        return initState;
    }

    var ops = {};

    switch (action.type) {
    case "startApp":
        ops = {
            "mainWindowVisible": { $set: true}
        };

        state = update(state, ops);
        break;

    case "load":
        var path = Url.path(action.source);
        ops = {
            source: {$set: action.source},
            fileName: {$set: Shell.basename(path)},
            folder: {$set: Shell.dirname(path)},
            mainWindowTitle: {$set: Shell.basename(path)}
        };
        state = update(state, ops);
        break;

    case "setSelectedState":
        ops = {
            selectedState: {$set: action.state}
        }
        state = update(state, ops);
        break;

    case "moveSelectedState":
        state = moveSelectedState(state, action);
        break;

    case "setAvailableState":
        var states = Lodash.map(action.states, function(name) {
            return {
                name: name,
                displayName: name
            }
        });
        states.unshift({ displayName: "<base state>" , name: ""});
        state = Lodash.assign({}, state, {availableStates: states});
        break;

    case "closeErrorPanel":
        state = Lodash.assign({}, state, {errorString: ""});
        break;

    case"setErrorString":
        state = Lodash.assign({}, state, {errorString: action.errorString});
        break;

    case "browse":
        state = Lodash.assign({}, state,
                              {
                                  browsingFolder: state.folder,
                                  views: [ {name: "browserPanel"} ]
                              });
        break;

    case "pop":
        state = Lodash.assign({}, state,
                              {
                                  views: []
                              });
        break;
    }

    return state;
}
