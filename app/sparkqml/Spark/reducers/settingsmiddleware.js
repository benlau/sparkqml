
.pragma library

function create(settings) {
    return function (store) {

        return function(next) {

            return function (action) {

                if (action.type === "startApp") {
                    var recentFiles = []

                    try {
                        recentFiles = JSON.parse(settings.recentFiles);
                    } catch(e) {
                    }

                    next({
                        type: "setRecentFiles",
                        files: recentFiles
                    });

                } else if (action.type === "addRecentFile" ||
                           action.type === "load") {
                    next(action);
                    // Save recent files to settings
                    settings.recentFiles = JSON.stringify(store.getState().recentFiles);
                    return;
                }

                next(action);
            }
        }
    }
}
