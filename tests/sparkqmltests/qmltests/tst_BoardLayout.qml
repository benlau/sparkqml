import QtQuick 2.0
import QtTest 1.0
import QUIKit 1.0

Item {

    TestCase {
        name: "BoardLayout"

        Item {
            id: item
            BoardPosition.row: 2
            BoardPosition.column: 3
        }

        function test_BoardPosition() {
            compare(item.BoardPosition.row, 2);
            compare(item.BoardPosition.column, 3);
        }

    }
}
