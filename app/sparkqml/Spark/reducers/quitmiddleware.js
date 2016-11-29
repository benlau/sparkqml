
.pragma library


function create() {

    return function (store) {

        return function(next) {

            return function (action) {
                next(action);
                if (action.type === "quitApp") {
                    Qt.quit();
                }
            }
        }
    }
}
