import QtQuick 2.0
import Spark.constants 1.0
import Spark.sys 1.0
import Shell 1.0
import QuickPromise 1.0
import Future 1.0

Item {
    id: component

    property string errorString: ""

    property string selectedState: ""

    property var availableStates: new Array

    property bool autoScanImportPathList: true

    property alias item: loader.item

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
        loader.source = "";
        Engine.clearComponentCache();
        Engine.errorString = "";
        if (autoScanImportPathList) {
            var path = Shell.dirname(Url.path(source));
            var future = Engine.scanImportPathList(path);
            Future.onFinished(future, function() {
                loader.source = source;
            });
        } else {
            Q.setTimeout(function() {
                loader.source = source;
            },0);
        }
    }

    function reload() {
        var source = loader.source;
        errorString = "";
        load(source);
    }

    Loader {
        id: loader
        anchors.centerIn: parent

        onStatusChanged:  {
            if (status === Loader.Error) {
                availableStates = [];
                errorString = Engine.errorString;
                component.error();
            } else if (status === Loader.Ready) {
                loader.item.anchors.centerIn = loader;
                var tmp = [];
                for (var i in loader.item.states) {
                    tmp.push(loader.item.states[i].name);
                }
                availableStates = tmp;
                scaleToBest();
                component.loaded();
            }
        }
    }

    Binding {
        target: loader.item
        property: "state"
        value: selectedState
    }

}
