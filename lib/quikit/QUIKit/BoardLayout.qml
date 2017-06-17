import QtQuick 2.0
import QUIKit 1.0
import QtQml.Models 2.2
import "./boardlayout.js" as BoardLayout

Item {
    id: component

    default property alias content: objectModel.children

    property var model: objectModel
    property int rows: 0
    property int columns: 0
    property int gridWidth: 0
    property int gridHeight: 0

    implicitWidth: columns * gridWidth
    implicitHeight: rows * gridHeight

    function refresh() {

        var items = [];
        var maxRows = 1;
        var maxColumns = 1;
        var maxItemWidth = 0;
        var maxItemHeight = 0;
        var i = 0;
        var item;
        var r, c;

        for (i = 0 ; i < repeater.count ; i++) {
            item = repeater.itemAt(i);
            if (item === null) {
                continue;
            }
            items.push(item);
            maxRows = Math.max(item.BoardPosition.row + 1, maxRows);
            maxColumns = Math.max(item.BoardPosition.column + 1, maxColumns);
            maxItemWidth = Math.max(item.width, maxItemWidth);
            maxItemHeight = Math.max(item.height, maxItemHeight);
        }

        while (maxRows * maxColumns < repeater.count) {
            if (maxRows < maxColumns) {
                maxRows++;
            } else {
                maxColumns++;
            }
        }

        var grid = BoardLayout.array2D(maxRows, maxColumns);

        // Mark assigned item
        for (i = 0 ; i < repeater.count ; i++) {
            item = items[i];
            r = item.BoardPosition.assignedRow;
            c = item.BoardPosition.assignedColumn;

            if (r < 0 || c < 0) {
                continue;
            }

            if (grid[r][c]) {
                // Unassign it
                item.BoardPosition.assignedRow = -1;
                item.BoardPosition.assignedColumn = -1;
            } else {
                grid[r][c] = item;
            }
        }

        // Assign item
        var iterator = new BoardLayout.Array2DIterator(grid);
        for (i = 0 ; i < repeater.count ; i++) {
            item = items[i];
            r = item.BoardPosition.assignedRow;
            c = item.BoardPosition.assignedColumn;

            if (r >= 0 && c >= 0) {
                continue;
            }

            var res = iterator.insert(item);
            item.BoardPosition.assignedRow = res.row;
            item.BoardPosition.assignedColumn = res.column;
        }

        // Set (x,y) per item
        for (i = 0 ; i < repeater.count ; i++) {
            item = items[i];
            r = item.BoardPosition.assignedRow;
            c = item.BoardPosition.assignedColumn;
            item.x = c * maxItemWidth;
            item.y = r * maxItemHeight;
        }

        gridWidth = maxItemWidth;
        gridHeight = maxItemHeight;
        rows = maxRows;
        columns = maxColumns;
    }

    ObjectModel {
        id: objectModel
    }

    Repeater {
        id: repeater
        model: component.model
    }

    Component.onCompleted: {
        refresh();
    }
}
