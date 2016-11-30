import QtQuick 2.4
import "../components"

import Spark.actions 1.0
import QuickFlux 1.0
import Spark.sys 1.0

MainComponentViewerForm {

    ComponentViewer {
        id: viewer;
        anchors.fill: parent

        selectedState: provider.selectedState

        onLoaded: {
            actions.forceActiveFocusOnStateListView();
            actions.setAvailableStates(viewer.availableStates);
        }

        onError: {
            actions.setErrorString(viewer.errorString);
        }
    }

    Connections {
        target: provider

        ignoreUnknownSignals: true

        onLoad: {
            viewer.load(provider.source);
        }

        onReload: {
            viewer.reload();
        }

        onScaleToFit: {
            viewer.scaleToFit();
        }

        onResizeToFit: {
            viewer.resizeToFit();
        }

        onCopyToClipboard: {
            viewer.item.grabToImage(function(result) {
                Clipboard.setImage(result.image);
            });
        }

        onCopyToFile: {
            viewer.item.grabToImage(function(result) {
                result.saveToFile(file);
            });
        }
    }
}
