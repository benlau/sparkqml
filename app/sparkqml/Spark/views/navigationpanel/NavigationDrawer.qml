import QtQuick 2.0
import QtQuick.Controls 2.0

Drawer {
    id: drawer
    dim: true
    dragMargin: 0
    width: 200

    NavigationPanel {
        anchors.fill: parent
    }

    Connections {
        target: provider
        ignoreUnknownSignals: true

        onCloseDrawer: {
            drawer.close();
        }

        onOpenDrawer: {
            drawer.open();
        }
    }
}
