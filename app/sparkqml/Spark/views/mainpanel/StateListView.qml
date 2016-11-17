import QtQuick 2.0
import Spark.actions 1.0

ListView {

    Keys.onUpPressed: {
        AppActions.moveSelectedState("up");
    }

    Keys.onDownPressed:  {
        AppActions.moveSelectedState("down");
    }

}
