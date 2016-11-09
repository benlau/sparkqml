import QtQuick 2.4
import QtQuick.Controls 2.0
import Spark.stores 1.0
import Spark.actions 1.0

MainPanelForm {
    id: component
    showErrorPanel: MainStore.errorString !== ""

    maskMouseArea.onClicked: {
        AppActions.closeErrorPanel();
    }


}
