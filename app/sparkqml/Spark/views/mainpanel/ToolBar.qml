import QtQuick 2.7
import QtQuick.Controls 1.4
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

    Shortcut {
        sequence: "Ctrl+R"
        onActivated: actions.reload();
    }

    reloadButton {
        onClicked: actions.reload();
    }

    barButton {
        onClicked: actions.openDrawer();
    }
}
