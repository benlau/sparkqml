import QtQuick 2.4
import Spark.actions 1.0

NavigationPanelForm {

    browse.onClicked: {
        AppActions.browse();
    }

    openMockup.onClicked: {
        AppActions.openMockupProject();
    }

    exitButton.onClicked: {
        AppActions.quitApp();
    }
}
