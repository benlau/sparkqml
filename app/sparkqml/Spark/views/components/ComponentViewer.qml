import QtQuick 2.0
import Spark.constants 1.0
import Spark.sys 1.0

Item {
    id: component

    property string errorMessage: ""

    property var componentStates: new Array

    property bool autoScanImportPathList: true

    signal loaded()
    signal error()

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

    function load(source) {
        Engine.clearComponentCache();
        loader.source = source;
    }

    function reload() {
        var source = loader.source;
        errorMessage = "";
        loader.source = "";
        load(source);
    }

    Loader {
        id: loader
        anchors.centerIn: parent

        onStatusChanged:  {
            if (status === Loader.Error) {
                componentStates = [];
                errorMessage = Engine.errorString;
                component.loaded();
            } else if (status === Loader.Ready) {
                loader.item.anchors.centerIn = loader;
                componentStates = loader.item.states;
                component.error();
            }
        }
    }

}
