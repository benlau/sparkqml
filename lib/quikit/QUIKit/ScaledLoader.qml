import QtQuick 2.0

Item {
    id: component

    property bool autoScaleEnabled: true

    property alias source : loader.source

    property alias soureComponent : loader.sourceComponent

    function scaleToFit() {
        var sw = (component.width / loader.item.width);
        var sh = (component.height / loader.item.height);
        var s = sw > sh ? sh : sw;
        loader.item.scale = s;
    }

    function refresh() {
        if (autoScaleEnabled) {
            scaleToFit();
        } else {
            component.width = loader.item.width;
            component.height = loader.item.height;
        }
    }

    onWidthChanged: {
        refresh();
    }

    onHeightChanged: {
        refresh();
    }

    Loader {
        id: loader
        anchors.centerIn: parent

        onStatusChanged: {
            refresh();
        }
    }
}
