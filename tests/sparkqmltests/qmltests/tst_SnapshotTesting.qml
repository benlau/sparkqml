import QtQuick 2.0
import QtTest 1.0
import SnapshotTesting 1.0

Item {

    TestCase {
        name: "SnapshotTesting"

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

        function test_capture() {
            var snapshot = SnapshotTesting.capture(item1);
            console.log(snapshot);
            compare(SnapshotTesting.matchStoredSnapshot("test_capture", snapshot), true);
        }

    }

}
