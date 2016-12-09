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
    mainWindowTitle: "SparkQML",
    source: "",
    fileName: "",
    folder: "",
    selectedState: "",
    errorString: "",
    browsingFolder: "",
    views: [],
    availableStates: [],
    recentFiles: []
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

function addRecentFile(state, action) {
    var recentFiles = state.recentFiles;

    var idx = Lodash.findIndex(recentFiles, function(item) {
        return item.file === action.file;
    });

    if (idx >= 0) {
        // remove then move to top
        recentFiles.splice(idx,1);
    }

    var item = {
        file: action.file,
        basename: Shell.basename(action.file)
    }

    recentFiles.unshift(item);

    recentFiles.splice(10,recentFiles.length - 10);

    state = Lodash.assign({}, state, {recentFiles: recentFiles});

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
            mainWindowTitle: {$set: Shell.basename(path)},
            errorString: {$set: ""}
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

    case "setAvailableStates":
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

    case "setRecentFiles":
        state = Lodash.assign({}, state, { recentFiles: action.files});
        break;

    case "addRecentFile":
        state = addRecentFile(state, action);
        break;
    }

    return state;
}
