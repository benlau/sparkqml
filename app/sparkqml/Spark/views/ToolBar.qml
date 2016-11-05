import QtQuick 2.4
import QtQuick.Controls 2.0
import Spark.actions 1.0

ToolBarForm {
    saveButton.onClicked: {
        AppActions.askToSaveFile();
    }

    clipboardButton.onClicked: {
        AppActions.copyToClipboard();
    }

    scaleToFitButton {
        onClicked: AppActions.scaleToFit();
    }

    refreshButton {
        onClicked: AppActions.refresh();
    }

    barButton {
        onClicked: AppActions.openDrawer();
    }
}
