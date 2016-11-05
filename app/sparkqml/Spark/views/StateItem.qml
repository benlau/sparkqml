import QtQuick 2.4
import Spark.actions 1.0
import Spark.stores 1.0

StateItemForm {

    property string name: "";

    mouseArea {
        onClicked: {
            AppActions.setState(name);
        }
    }

    selected: MainStore.selectedState == name;

}
