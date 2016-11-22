.pragma library
.import "../js/immutabilityhelper/immutabilityhelperwrapper.js" as ImmutabilityHelper
.import Shell 1.0 as ShellWrapper
.import Spark.sys 1.0 as SparkSys

var update = ImmutabilityHelper;
var Shell = ShellWrapper.Shell;
var Url = SparkSys.Url;

var initState = {
    mainWindowVisible: false,
    mainWindowTitle: "Spark",
    source: "",
    fileName: "",
    folder: "",
    selectedState: ""
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
    }

    return state;
}
