import QtQuick 2.4
import QtQuick.Controls 2.0
import Spark.actions 1.0

ToolBarForm {
    resizeButton.onClicked: {
        AppActions.resizeToFit();
    }

    saveButton.onClicked: {
        AppActions.askToSaveFile();
    }

    clipboardButton.onClicked: {
        AppActions.copyToClipboard();
    }

    scaleToFitButton {
        onClicked: AppActions.scaleToFit();
    }

    reloadButton {
        onClicked: AppActions.reload();
    }

    barButton {
        onClicked: AppActions.openDrawer();
    }
}
