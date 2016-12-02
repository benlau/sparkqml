import QtQuick 2.4
import Spark.actions 1.0

NavigationPanelForm {

    browse.onClicked: {
        actions.browse();
    }

    openMockup.onClicked: {
        actions.closeDrawer();
        actions.openMockupProject();
    }

    exitButton.onClicked: {
        actions.quitApp();
    }
}
