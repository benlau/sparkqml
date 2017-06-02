import QtQuick 2.4
import QtQuick.Layouts 1.1

Item {
    id : component

    default property alias content: container.children

    property alias name : text.text

    /// If it is enabled, NameTag's size will grow to fit content.
    /// Otherwise it will only use the assigned area
    property bool growToFitContent: false

    property int nameTextHeight: 30

    /// The font of name text
    property alias font : text.font

    signal refreshed();

    function refresh() {
        if (growToFitContent) {
            component.width = Math.max(container.width, fontMetrics.boundingRect(name).width);
            component.height = container.height + nameTextHeight;
        } else {
            container.width = holder.width;
            container.height = holder.height;
            if (container.children.length > 0) {
                container.children[0].width = holder.width;
                container.children[0].height = holder.height;
            }
        }
        refreshed();
    }

    onNameChanged: {
        refresh();
    }

    onWidthChanged: {
        refresh();
    }

    onHeightChanged: {
        refresh();
    }

    FontMetrics {
      id: fontMetrics
      font : text.font
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
            Layout.maximumHeight: nameTextHeight
            color: "#de000000"
            horizontalAlignment: Text.AlignHCenter
            font.pixelSize: 14
            verticalAlignment: Text.AlignVCenter
        }
    }
}
