import QtQuick 2.4
import Spark.actions 1.0


StateItemForm {
    id: component

    property string name: "";

    mouseArea {
        onClicked: {
            actions.setSelectedState(name);
            component.forceActiveFocus();
        }
    }

    selected: provider.selectedState === name;

}
