import QtQuick 2.0

Item {
    id : shadow

    property int depth : 1;

    Shadow {
        id: bottomShadow
        anchors.fill: parent
    }

    Shadow {
        id: topShadow
        anchors.fill: parent
    }

    // Reference: http://www.google.com/design/spec/layout/layout-principles.html#layout-principles-dimensionality

    states: [
        State {
            name: "D1"
            when : depth === 1

            PropertyChanges {
                target: topShadow
                verticalOffset : 1
                horizontalOffset: 0
                opacity: 0.12
                blur : 1.5
            }

            PropertyChanges {
                target: bottomShadow
                verticalOffset : 1
                horizontalOffset: 0
                blur : 1
                opacity: 0.24
            }

        },
        State {
            name: "D2"
            when : depth === 2

            PropertyChanges {
                target: topShadow
                verticalOffset : 3
                horizontalOffset: 0
                blur : 3
                opacity: 0.16
            }

            PropertyChanges {
                target: bottomShadow
                verticalOffset : 3
                horizontalOffset: 0
                blur : 3
                opacity: 0.23
            }
        },
        State {
            name: "D3"
            when : depth === 3

            PropertyChanges {
                target: topShadow
                verticalOffset : 10
                horizontalOffset: 0
                blur : 10
                opacity: 0.19
            }

            PropertyChanges {
                target: bottomShadow
                verticalOffset : 6
                horizontalOffset: 0
                blur : 3
                opacity: 0.23
            }
        },

        State {
            name: "D4"
            when : depth === 4

            PropertyChanges {
                target: topShadow
                verticalOffset : 14
                horizontalOffset: 0
                blur : 14
                opacity: 0.25
            }

            PropertyChanges {
                target: bottomShadow
                verticalOffset : 10
                horizontalOffset: 0
                blur : 5
                opacity: 0.22
            }
        },
        State {
            name: "D5"
            when : depth === 5

            PropertyChanges {
                target: topShadow
                verticalOffset : 19
                horizontalOffset: 0
                blur : 19
                opacity: 0.30
            }

            PropertyChanges {
                target: bottomShadow
                verticalOffset : 15
                horizontalOffset: 0
                blur : 6
                opacity: 0.22
            }
        }
    ]
}
