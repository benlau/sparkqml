import QtQuick 2.0
import QtTest 1.0
import SnapshotTesting 1.0

Item {

    Item {
        id: item1

        width: 100
        height: 100

        Item {
            id: child1
            width: 100
            height: 100
        }
    }

    TestCase {
        name: "SnapshotTesting"
        when: windowShown


        function test_capture() {
            console.log(item1.visible)
            var snapshot = SnapshotTesting.capture(item1);
            console.log(snapshot);
            compare(SnapshotTesting.matchStoredSnapshot("test_capture", snapshot), true);
        }

    }

}
