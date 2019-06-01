.pragma library
.import "qrc:/QuickPromise/promise.js" as Q
.import QUIKit 1.0 as QUIKit
.import Spark.sys 1.0 as SparkSys
.import Shell 1.0 as ShellPkg

var Shell = ShellPkg.Shell;
var Url = SparkSys.Url;
var FileInfo = SparkSys.FileInfo;

/// Given a qml file. Convert to an image file name.
function convertToImageName(fileName, selectedState) {
    var base = FileInfo.completeBaseName(fileName);
    if (selectedState === "") {
        return base + ".png";
    } else {
        return base + "(" + selectedState + ").png";
    }
}

function askToSaveFile(dialog, settings, store) {
    var promise = Q.promise();
    promise.resolve(dialog.onAccepted);
    promise.reject(dialog.onRejected);
    promise.then(function() {
        var path = Url.path(dialog.file.toString());
        store.dispatch({
            type: "copyToFile",
            arguments: [path]
        });
    });
    var folder = settings.saveFolder;
    var state = store.getState();

    if (folder === "") {
        folder = Shell.dirname(Url.path(state.source));
    }
    var fileName = convertToImageName(state.fileName, state.selectedState);;
    folder = "file:" + folder;
    var fileUrl = folder + "/" + fileName;

    dialog.nameFilters = [ "Image files (*.png)"];
    dialog.folder = folder;
    dialog.currentFile = fileUrl;
    dialog.open();
}

function create(context, settings) {

    return function (store) {

        return function(next) {

            return function (action) {
                if (action.type !== "askToSaveFile") {
                    next(action);
                    return
                }
                askToSaveFile(context.mainFileDialog, settings, store);
            }
        }
    }
}
