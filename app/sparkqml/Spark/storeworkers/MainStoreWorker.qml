import QtQuick 2.0
import QuickFlux 1.0
import Spark.constants 1.0
import Spark.actions 1.0
import Spark.stores 1.0
import QuickPromise 1.0
import Future 1.0
import Shell 1.0
import Spark.sys 1.0
import "./lodash.min.js" as Lodash

StoreWorker {
    id: worker

    function scaleToFit() {
        var sw = (mainContext.mainViewer.width  - Constants.viewerPadding * 2) / loader.item.width;
        var sh = (mainContext.mainViewer.height  - Constants.viewerPadding * 2) / loader.item.height;
        var s = sw > sh ? sh : sw;
        loader.item.scale = s;
    }

    function scaleToBest() {
        if (loader.item.width > mainContext.mainViewer.width - Constants.viewerPadding * 2  ||
            loader.item.height >  mainContext.mainViewer.height - Constants.viewerPadding * 2) {
            scaleToFit();
        }
    }

    function resizeToFit() {
        loader.item.width = Qt.binding(function() { return mainContext.mainViewer.width - Constants.viewerPadding * 2});
        loader.item.height = Qt.binding(function() { return mainContext.mainViewer.height - Constants.viewerPadding * 2});
    }

    /// Given a qml file. Convert to an image file name.
    function convertToImageName(fileName, selectedState) {
        var base = FileInfo.completeBaseName(fileName);
        if (selectedState === "") {
            return base + ".png";
        } else {
            return base + "(" + selectedState + ").png";
        }
    }

    Filter {
        type: ActionTypes.startApp
        onDispatched: {
            mainWindow.visible = true;
        }
    }

    Filter {
        type: ActionTypes.quitApp
        onDispatched: {
            Qt.quit();
        }
    }

    Loader {
        id: loader
    }

    function load(source) {
        var path = Url.path(source);
        MainStore.source = source;
        MainStore.folder = Shell.dirname(path);
        MainStore.fileName = Shell.basename(path);
        MainStore.selectedState = "";
        MainStore.title = MainStore.fileName ;

        scanImportPathList(source, function(){
            setSource(source);
        });
    }

    function scanImportPathList(source, callback) {
        var future = Engine.scanImportPathList(source);
        Future.onFinished(future, callback)
    }

    function setSource(source) {
        Engine.errorString = ""; // Guarantee Engine is initialized.
        loader.source = source;

        if (loader.status === Loader.Error){
            MainStore.states = [];
            MainStore.errorString = Engine.errorString;
            return;
        }

        loader.item.parent = mainContext.mainViewer;
        loader.item.anchors.centerIn = loader.item.parent;
        scaleToBest();
        var states = Lodash._.map(loader.item.states, function(state) {
            return {
                name: state.name,
                displayName: state.name
            }
        });
        states.unshift({ displayName: "<base state>" , name: ""});
        MainStore.states = states;
    }

    Filter {
        type: ActionTypes.load
        onDispatched: {
            load(message.source)
        }
    }

    Filter {
        type: ActionTypes.setState
        onDispatched: {
            loader.item.state = message.state;
            MainStore.selectedState = message.state;
        }
    }

    Filter {
        type: ActionTypes.reload
        onDispatched: {
            if (MainStore.source === "") {
                return;
            }
            loader.source = "";
            MainStore.errorString = "";
            Engine.clearComponentCache();
            AppActions.load(MainStore.source);
        }
    }

    Filter {
        type: ActionTypes.openDrawer
        onDispatched: {
            mainDrawer.open();
        }
    }

    Filter {
        type: ActionTypes.openMockupProject
        onDispatched: {
            mainDrawer.close();
        }
    }

    Filter {
        type: ActionTypes.scaleToFit
        onDispatched: {
            scaleToFit();
        }
    }

    Filter {
        type: ActionTypes.resizeToFit
        onDispatched: {
            resizeToFit();
        }
    }

    Filter {
        type: ActionTypes.copyToClipboard
        onDispatched: {
            loader.item.grabToImage(function(result) {
                Clipboard.setImage(result.image);
            });
        }
    }

    Filter {
        type: ActionTypes.askToSaveFile
        onDispatched: {
            var promise = Q.promise();
            promise.resolve(mainFileDialog.onAccepted);
            promise.reject(mainFileDialog.onRejected);
            promise.then(function() {
                mainSettings.saveFolder = Shell.dirname(mainFileDialog.fileUrl);
                loader.item.grabToImage(function(result) {
                    var path = Url.path(mainFileDialog.fileUrl.toString());
                    result.saveToFile(path);
                });
            });

            var folder = mainSettings.saveFolder;
            if (folder === "") {
                folder = Shell.dirname(Url.path(MainStore.source));
            }
            folder = "file:" + folder + "/" + convertToImageName(MainStore.fileName, MainStore.selectedState);

            mainFileDialog.selectMultiple = false;
            mainFileDialog.selectExisting = false;
            mainFileDialog.nameFilters = [ "Image files (*.png)"];
            mainFileDialog.folder = folder;
            mainFileDialog.open();
        }
    }

    Filter {
        type: ActionTypes.closeErrorPanel
        onDispatched: {
            MainStore.errorString = "";
        }
    }

}
