import QtQuick 2.0
import Spark.constants 1.0

Item {
    id: component

    property string source: ""

    width: 400
    height: 400

    function scaleToFit() {
        var sw = (component.width  - Constants.viewerPadding * 2) / loader.item.width;
        var sh = (component.height  - Constants.viewerPadding * 2) / loader.item.height;
        var s = sw > sh ? sh : sw;
        loader.item.scale = s;
    }

    function scaleToBest() {
        if (loader.item.width > component.width - Constants.viewerPadding * 2  ||
            loader.item.height >  component.height - Constants.viewerPadding * 2) {
            scaleToFit();
        }
    }

    function resizeToFit() {
        loader.item.width = Qt.binding(function() { return component.width - Constants.viewerPadding * 2});
        loader.item.height = Qt.binding(function() { return component.height - Constants.viewerPadding * 2});
        loader.item.scale = 1;
    }

    Loader {
        id: loader
        anchors.centerIn: parent
        source: component.source
    }

}
