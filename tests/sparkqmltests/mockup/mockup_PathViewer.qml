// Spark's mockup can not use package name due to the conflict with current build binary
import QtQuick 2.0
import "../../../app/sparkqml/Spark/views/browserpanel"
import "../../../app/sparkqml/Spark/views/navigationpanel"
import "../../../app/sparkqml/Spark/reducers"

Rectangle {
    id: window
    width: 480
    height: 640
    color: "gray"


    Column {

        PathViewer {
            width: window.width
        }

        PathViewer {
            width: window.width
            path: ["home", "sparkqml"]
        }

        PathViewer {
            width: window.width
            path: ["home", "sparkqml" ,"tests"]
        }


    }

}
