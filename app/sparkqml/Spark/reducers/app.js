.pragma library
.import "../js/immutabilityhelper/immutabilityhelperwrapper.js" as ImmutabilityHelper

var update = ImmutabilityHelper;

var initState = {
    mainWindowVisible: false
}

function reducer(state, actions) {
    if (state === undefined) {
        return initState;
    }

    switch (actions.type) {
    case "startApp":
        var ops = {
            "mainWindowVisible": { $set: true}
        };

        state = update(state, ops);
        break;
    }

    return state;
}
