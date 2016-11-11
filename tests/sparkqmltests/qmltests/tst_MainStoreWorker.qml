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


    }
}
