import QtQuick 2.6

Item {
    id: component

    default property alias content: container.children

    property bool enlarge: true

    function scaleToFit() {
        var sw = (component.width / container.width);
        var sh = (component.height / container.height);
        var s = Math.min(sw, sh);
        container.scale = s;
    }

    function refresh() {
        if (!enlarge && component.width >= container.width && component.height >= container.height) {
            container.scale = 1;
        } else {
            scaleToFit();
        }
    }

    Row {
        id: container
        anchors.centerIn: parent
        spacing: 0

        onWidthChanged: refresh();
        onHeightChanged: refresh();
    }

}
