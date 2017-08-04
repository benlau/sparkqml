import QtQuick 2.0
import QtTest 1.0
import SnapshotTesting 1.0

Item {
    width: 640
    height: 480

    Item {
        id: item1

        width: 100
        height: 100

        Item {
            id: child1
            width: 100
            height: 100
        }

        Rectangle {
            color: "red"
            anchors.fill: parent
            opacity: mouseArea.pressed ? 0.5 : 1
        }

        MouseArea {
            id: mouseArea
            anchors.fill: parent
        }
    }

    TestCase {
        name: "SnapshotTesting"
        when: windowShown


        function test_capture() {
            var snapshot = SnapshotTesting.capture(item1);
            console.log(snapshot);
            compare(SnapshotTesting.matchStoredSnapshot("test_capture", snapshot), true);
        }

    }

}
