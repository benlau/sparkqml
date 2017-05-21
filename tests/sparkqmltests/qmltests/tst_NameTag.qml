import QtQuick 2.0
import QtTest 1.0
import Spark.sys 1.0
import QUIKit 1.0
import Spark.views.components 1.0
import "../sample/rectanlges"

TestCase {
    name: "NameTag"
    when: windowShown

    NameTag {
        id: tag1

        Red100x100 {
            id: red1
        }
    }

    NameTag {
        id: tag2
        growToFitContent: true
        nameTextHeight: 20

        Loader {
            id: loader2
            sourceComponent: Blue100x50 {
            }
        }
    }

    function test_size() {
        compare(tag1.width, 0);
        compare(tag2.width, 100);
        compare(tag2.height, 70);

        tag2.name = "A super long name. A super long name. A super long name.";
        compare(tag2.width > 100, true);
    }

}
