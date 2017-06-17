.pragma library

function array2D(row, col) {
    var res = [];
    for (var i = 0 ; i < row ;i++) {
        var list = []
        for (var j = 0 ; j < col ; j++) {
            list.push(null);
        }
        res.push(list);
    }
    return res;
}

function Array2DIterator(arr) {
    this.array = arr;
    this.currentRow = 0;
    this.currentColumn = 0;
}

Array2DIterator.prototype.insert = function(item) {
    var array = this.array;
    var currentRow = this.currentRow;
    var currentColumn = this.currentColumn

    while (array[currentRow][currentColumn] !== null) {
        currentColumn++;
        if (currentColumn >= array[currentRow].length) {
            currentColumn = 0;
            currentRow++;
        }
    }

    array[currentRow][currentColumn] = item;
    this.currentRow = currentRow;
    this.currentColumn = currentColumn;
    return {
        row: currentRow,
        column: currentColumn
    }
}
