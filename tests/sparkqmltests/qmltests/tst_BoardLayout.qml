import QtQuick 2.0
import QtTest 1.0
import QUIKit 1.0
import "../../../lib/quikit/QUIKit/boardlayout.js" as BoardLayoutLib
Item {

    TestCase {
        name: "BoardLayout"
        when: windowShown

        Item {
            id: item
            BoardPosition.row: 2
            BoardPosition.column: 3
        }

        function test_BoardPosition() {
            compare(item.BoardPosition.row, 2);
            compare(item.BoardPosition.column, 3);
        }

        BoardLayout {
            id: layout1

            Item {
                id: layout1_item1
                width: 50
                height: 20
            }

            Item {
                id: layout1_item2
                width: 50
                height: 20
            }

            Item {
                id: layout1_item3
                BoardPosition.row: 3
                BoardPosition.column: 3
            }
        }

        function test_layout1() {
            var arr = BoardLayoutLib.array2D(5,6);
            compare(arr.length, 5);
            compare(arr[0].length, 6);

            compare(layout1.rows, 4);
            compare(layout1.columns, 4);

            compare(layout1_item1.x, 0);
            compare(layout1_item1.y, 0);

            compare(layout1_item1.BoardPosition.assignedRow , 0);
            compare(layout1_item1.BoardPosition.assignedColumn , 0);

            compare(layout1_item2.BoardPosition.assignedRow , 0);
            compare(layout1_item2.BoardPosition.assignedColumn , 1);
        }

    }
}
