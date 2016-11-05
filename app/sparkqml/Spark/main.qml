import QtQuick 2.3
import QtQuick.Window 2.2
import QtMultimedia 5.5
import QtQuick.Controls 1.4
import "./constants"
import "./adapters"
import "./actions"
import "./stores"

Window {
    id: mainWindow
    visible: false
    width: 480
    height: 640

    StoreAdapter {
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            AppActions.quitApp();
        }
    }

    Text {
        text: qsTr(MainStore.text)
        anchors.centerIn: parent
    }

}
