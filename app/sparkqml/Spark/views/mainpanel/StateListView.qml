import QtQuick 2.0
import Spark.actions 1.0

ListView {
    id: component

    Keys.onUpPressed: {
        actions.moveSelectedState("up");
    }

    Keys.onDownPressed:  {
        actions.moveSelectedState("down");
    }

    Connections {
        target: provider
        ignoreUnknownSignals: true
        onForceActiveFocusOnStateListView: {
            component.forceActiveFocus();
        }
    }

}
