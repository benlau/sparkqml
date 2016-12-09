
.pragma library

function create(settings) {
    return function (store) {

        return function(next) {

            return function (action) {

                if (action.type === "startApp") {

                    next({
                        type: "setRecentFiles",
                        files: settings.recentFiles
                    });

                } else if (action.type === "addRecentFile" ||
                           action.type === "load") {
                    next(action);
                    settings.recentFiles = store.getState().recentFiles;
                    return;
                }

                next(action);
            }
        }
    }
}
