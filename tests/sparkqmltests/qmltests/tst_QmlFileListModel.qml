import QtQuick 2.0
import QtTest 1.0
import Spark.sys 1.0
import QUIKit 1.0
import Spark.views.components 1.0
import "../sample/rectanlges"

TestCase {
    name: "QmlFileListModel"

    TestUtils {
        id: utils
    }

    Component {
        id: creator
        QmlFileListModel {

        }
    }

    function test_options() {
        console.log("test_options");
        var model = creator.createObject();

        model.folder = Qt.resolvedUrl("../sample/rectanlges/");
        model.options = {
            "Blue100x50": {
                color: "green"
            }
        }

        utils.waitUntil(function() {
            return model.count > 0;
        }, 1000)

        var item = model.get(0);
        compare(item.title, "Blue100x50");
        compare(item.properties.color, "green");

    }

}
