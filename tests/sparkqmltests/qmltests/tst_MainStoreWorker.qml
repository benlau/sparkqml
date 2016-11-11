import QtQuick 2.0
import QtTest 1.0
import Spark.storeworkers 1.0
import Spark.stores 1.0
import Spark.sys 1.0
import Shell 1.0

Item {

    TestCase {
        name: "MainStoreWorker"

        MainStoreWorker {
            id: worker
        }

        function init() {
            MainStore.source = "";
            MainStore.states = [];
            MainStore.title = "";
            MainStore.errorString = "";
        }

        function test_reload_withError() {
            worker.dispatched("load",{ source: Qt.resolvedUrl("../sample/SyntaxError.qml")});
            wait(1000); // It will search for qmlimport.path. It should extract the code into a new component
            compare(MainStore.errorString !=="", true);
            worker.dispatched("reload", {});

            // Before it is done, it should clear the errorString
            compare(MainStore.errorString === "", true);
            wait(1000);
        }

        function test_MainStore_folder() {
            var source = Qt.resolvedUrl("../sample/Rect.qml");
            var folder = Shell.dirname(Url.path(source));
            worker.dispatched("load", {source: source});
            compare(MainStore.source, source);
            compare(MainStore.folder, folder);
        }
    }
}
