import QtQuick 2.4
import "../components"
import Spark.stores 1.0
import Spark.actions 1.0
import QuickFlux 1.0
import Spark.sys 1.0

MainComponentViewerForm {

    ComponentViewer {
        id: viewer;
        anchors.fill: parent

        selectedState: MainStore.selectedState

        onLoaded: {
            AppActions.setAvailableStates(viewer.availableStates);
        }

        onError: {
            AppActions.setErrorString(viewer.errorString);
        }
    }

    Connections {
        target: MainStore

        onSourceChanged: {
            viewer.load(MainStore.source);
        }
    }

    AppListener {
        Filter {
            type: ActionTypes.reload
            onDispatched: {
                viewer.reload();
            }
        }

        Filter {
            type: ActionTypes.scaleToFit
            onDispatched: {
                viewer.scaleToFit();
            }
        }

        Filter {
            type: ActionTypes.resizeToFit
            onDispatched: {
                viewer.resizeToFit();
            }
        }

        Filter {
            type: ActionTypes.copyToClipboard
            onDispatched: {
                viewer.item.grabToImage(function(result) {
                    Clipboard.setImage(result.image);
                });
            }
        }

        Filter {
            type: ActionTypes.copyToFile
            onDispatched: {
                viewer.item.grabToImage(function(result) {
                    result.saveToFile(message.target);
                });
            }
        }

    }


}
