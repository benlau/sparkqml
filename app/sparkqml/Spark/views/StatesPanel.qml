import QtQuick 2.4
import Spark.stores 1.0

StatesPanelForm {

    listView {
        model: MainStore.states
    }

}
