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
        MainStore.fileName = Shell.basename(path);
        MainStore.folder = Shell.dirname(path);
        MainStore.selectedState = "";
        MainStore.title = MainStore.fileName ;
    }

    Filter {
        type: ActionTypes.setAvailableStates
        onDispatched: {
            var states = Lodash._.map(message.states, function(name) {
                return {
                    name: name,
                    displayName: name
                }
            });
            states.unshift({ displayName: "<base state>" , name: ""});
            MainStore.states = states;
        }
    }

    Filter {
        type: ActionTypes.load
        onDispatched: {
            load(message.source)
        }
    }

    Filter {
        type: ActionTypes.setSelectedState
        onDispatched: {
            MainStore.selectedState = message.state;
        }
    }

    Filter {
        type: ActionTypes.moveSelectedState
        onDispatched: {
            var index = -1;
            for (var i = 0 ; i < MainStore.states.length;i++) {
                var state = MainStore.states[i];
                if (state.name === MainStore.selectedState) {
                    index = i;
                    break;
                }
            }

            if (message.direction === "up") {
                index--;
            } else {
                index++;
            }

            if (index < 0) {
                index = 0;
            }
            if (index >= MainStore.states.length) {
                index = MainStore.states.length - 1;
            }

            MainStore.selectedState = MainStore.states[index].name;
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

    // @TODO - Extract to another worker
    Filter {
        type: ActionTypes.askToSaveFile
        onDispatched: {
            var promise = Q.promise();
            promise.resolve(mainFileDialog.onAccepted);
            promise.reject(mainFileDialog.onRejected);
            promise.then(function() {
                var path = Url.path(mainFileDialog.fileUrl.toString());
                AppActions.copyToFile(path);
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

    Filter {
        type: ActionTypes.setErrorString
        onDispatched: {
            MainStore.errorString = message.errorString
        }
    }

    Filter {
        type: ActionTypes.reload
        onDispatched: {
            MainStore.errorString = "";
            MainStore.reloadTrigger();
        }
    }

    Filter {
        type: ActionTypes.browse
        onDispatched: {
            mainDrawer.close();
            MainStore.browsingFolder = MainStore.folder;
            MainStore.views = [
                {
                    name: "browserPanel"
                }
            ]
        }
    }

    Filter {
        type: ActionTypes.pop
        onDispatched: {
            MainStore.views = [];
        }
    }
}
