import QtQuick 2.0
import QtQml.Models 2.2
import QUIKit 1.0

Item {
    width: 320
    height: 480

    ObjectModel {
        id: objectModel

        Rectangle {
            width: 100
            height: 100
            color: "red"
        }

        Rectangle {
            width: 90
            height: 50
            color: "blue"
        }

        Rectangle {
            width: 10
            height: 10
            color: "green"
        }

        Rectangle {
            width: 300
            height: 200
            color: "red"
        }

        Rectangle {
            width: 50
            height: 100
            color: "blue"
        }
    }

    BoxPackingFlow {
        width: parent.width
        model: objectModel
    }

}
