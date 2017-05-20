import QtQuick 2.0
import QtQuick.Layouts 1.1

Item {
    id : component

    default property alias content: container.children

    property alias name : text.text

    property bool grow: false

    function refresh() {
        if (grow) {
            component.width = container.width;
            component.height = container.height + 20;
        } else {
            container.width = holder.width;
            container.height = holder.height;
            if (container.children.length > 0) {
                container.children[0].width = holder.width;
                container.children[0].height = holder.height;
            }
        }
    }

    onWidthChanged: {
        refresh();
    }

    onHeightChanged: {
        refresh();
    }

    ColumnLayout  {
        anchors.fill: parent
        anchors.margins: 4

        Item {
            id: holder
            Layout.fillWidth: true
            Layout.fillHeight: true

            Grid {
                id: container
                anchors.centerIn: parent
                onWidthChanged: refresh();
                onHeightChanged: refresh();
            }
        }

        Text {
            id: text
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.maximumHeight: 20
            color: "#de000000"
            horizontalAlignment: Text.AlignHCenter
            font.pixelSize: 14
            elide: Text.ElideMiddle
            verticalAlignment: Text.AlignVCenter
        }
    }
}
