import QtQuick 2.4
import QtQuick.Controls 2.0
import Spark.actions 1.0

ToolBarForm {
    resizeButton.onClicked: {
        actions.resizeToFit();
    }

    saveButton.onClicked: {
        actions.askToSaveFile();
    }

    clipboardButton.onClicked: {
        actions.copyToClipboard();
    }

    scaleToFitButton {
        onClicked: actions.scaleToFit();
    }

    reloadButton {
        onClicked: actions.reload();
    }

    barButton {
        onClicked: actions.openDrawer();
    }
}
