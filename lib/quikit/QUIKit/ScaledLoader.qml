import QtQuick 2.0

Item {
    id: component

    property bool autoScaleToFit: true

    property alias source : loader.source

    property alias soureComponent : loader.sourceComponent

    property var properties: null

    function scaleToFit() {
        var sw = (component.width / loader.item.width);
        var sh = (component.height / loader.item.height);
        var s = sw > sh ? sh : sw;
        loader.item.scale = s;
    }

    function refresh() {
        if (autoScaleToFit) {
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
            if (loader.status === Loader.Ready) {
                if (component.properties) {
                    for (var i in component.properties) {
                        loader.item[i] = component.properties[i];
                    }
                }
            }
            refresh();
        }
    }
}
