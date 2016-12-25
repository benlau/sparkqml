import QtQuick 2.4
import Spark.actions 1.0

BrowserActionBarForm {

    pathViewer {
        path: {
            return provider.browsingFolder.split("/");
        }
    }

    closeButton.onClicked: {
        actions.pop();
    }
}
