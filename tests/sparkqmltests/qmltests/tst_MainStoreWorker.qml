import QtQuick 2.0
import QtTest 1.0
import Spark.workers 1.0
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
            MainStore.errorString = "Error";
            worker.dispatched("reload", {});
            compare(MainStore.errorString, "");
        }
    }
}
