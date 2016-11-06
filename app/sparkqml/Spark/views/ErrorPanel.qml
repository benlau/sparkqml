import QtQuick 2.4
import Spark.stores 1.0
import Spark.actions 1.0

ErrorPanelForm {
    message: MainStore.errorString

    confirmButton.onClicked: {
        AppActions.closeErrorPanel();
    }
}
