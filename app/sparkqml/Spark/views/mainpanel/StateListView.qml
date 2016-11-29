import QtQuick 2.0
import Spark.actions 1.0

ListView {

    Keys.onUpPressed: {
        actions.moveSelectedState("up");
    }

    Keys.onDownPressed:  {
        actions.moveSelectedState("down");
    }

}
