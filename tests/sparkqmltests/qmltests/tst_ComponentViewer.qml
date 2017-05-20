import QtQuick 2.0
import QtTest 1.0

import Spark.sys 1.0
import QUIKit 1.0
import Spark.views.components 1.0

Item {

    TestCase {
        name: "ComponentViewer"

        Component {
            id: viewerCreator
            ComponentViewer {
            }
        }

        function waitUntilTimeout(callback, timeout) {
            if (timeout === undefined) {
                timeout = 1000;
            }

            while (!callback() && timeout > 0) {
                wait(10);
                timeout -= 10;
            }

            return timeout > 0;
        }

        function test_reload_withError() {
            var viewer = viewerCreator.createObject();
            viewer.load(Qt.resolvedUrl("../sample/SyntaxError.qml"));
            compare(viewer.errorString, "");

            compare(waitUntilTimeout(function() {
               return viewer.errorString == ""
            }), true);

            viewer.reload();
            // Before it is done, it should clear the errorString
            compare(viewer.errorString === "", true);

            compare(waitUntilTimeout(function() {
               return viewer.errorString == ""
            }), true);
            viewer.destroy();
        }

    }
}
