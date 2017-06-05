import QtQuick 2.0

Item {
    id: component

    property alias model: repeater.model
    property alias delegate: repeater.delegate

    property int spacing: 10

    property int padding: 10

    function refresh() {
        var items = [];

        for (var i = 0 ; i < repeater.count ; i++) {
            var item = repeater.itemAt(i);
            if (item === null) {
                continue;
            }

            items.push(item);
        }

        items.sort(function(a,b) {
           return a.height - b.height;
        });

        var scanline = padding;
        var box = { // The box size of current line
            width: 0,
            height: 0
        }

        for (var i = 0 ; i < items.length ; i++) {
            var item = items[i];

            if (box.width + item.width < (component.width - (padding * 2)) ) {
                item.x = box.width;
                item.y = scanline;
                box.width += item.width + spacing;
                box.height = Math.max(box.height, item.height);
            } else {
                // Place it on next line
                var nextScanline = scanline + box.height + spacing;
                item.x = padding;
                item.y = nextScanline;
                box.width = padding + item.width + spacing;
                box.height = item.height;
                scanline = nextScanline;
            }
        }

        component.height = scanline + box.height;
    }

    Repeater {
        id: repeater

        Component.onCompleted: {
            refresh();
        }

        onItemAdded: {
            refresh();
        }

        onItemRemoved: {
            refresh();
        }
    }

}
