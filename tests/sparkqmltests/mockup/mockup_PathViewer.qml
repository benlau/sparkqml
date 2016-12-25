// Spark's mockup can not use package name due to the conflict with current build binary
import QtQuick 2.0
import "../../../app/sparkqml/Spark/views/browserpanel"
import "../../../app/sparkqml/Spark/views/navigationpanel"
import "../../../app/sparkqml/Spark/reducers"

Rectangle {
    id: window
    width: 360
    height: 640
    color: "gray"


    Column {

        PathViewer {
        }

        PathViewer {
            path: ["", "", ""]
        }

        PathViewer {
            width: window.width
        }

        PathViewer {
            width: window.width
            path: ["home", "sparkqml"]
        }

        PathViewer {
            width: window.width
            path: ["home", "sparkqml" ,"src"]
        }

        PathViewer {
            // It could show as most 3 at a time
            width: window.width
            path: ["home", "sparkqml" ,"tests", "sparktests"]
        }

        PathViewer {
            // It could show as most 3 at a time
            width: window.width
            path: ["home", "sparkqml" ,"tests", "sparktests - super long file name."]
        }

    }

}
