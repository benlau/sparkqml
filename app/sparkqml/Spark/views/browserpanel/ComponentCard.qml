import QtQuick 2.4
import Spark.actions 1.0

ComponentCardForm {
    id: component
    mouseArea.onClicked: {
        AppActions.pop();
        AppActions.load(component.source);
    }
}
