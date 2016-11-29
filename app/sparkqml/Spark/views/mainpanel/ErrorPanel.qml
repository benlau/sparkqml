import QtQuick 2.4

import Spark.actions 1.0

ErrorPanelForm {
    message: provider.errorString

    reloadButton.onClicked: {
        actions.closeErrorPanel();
        actions.reload();
    }

    confirmButton.onClicked: {
        actions.closeErrorPanel();
    }
}
