/* Sample3 - Test Repeater
 */
import QtQuick 2.0

Item {
    width: 640
    height: 480

    Column {
        id: column
        Repeater {
            model: 5
            delegate: Item {
                width: 640
                height: 48
            }
        }
    }

    ListView {
        model: 5
        delegate: Item {
            width: 640
            height: 48
        }
    }
}
