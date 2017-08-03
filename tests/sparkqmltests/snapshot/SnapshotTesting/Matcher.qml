import QtQuick 2.4
import QtQuick.Controls 1.2
import QtQuick.Dialogs 1.2

Dialog {
    id: component

    visible: false
    title: "Snapshot Testing"
    modality: Qt.NonModal

    property alias diff: content.diff
    property alias previousSnapshot: content.originalVersion
    property alias snapshot: content.currentVersion
    property alias screenshot: content.screenshot

    MatcherContentView {
        id: content
        implicitWidth: 640
        implicitHeight: 480
    }

    onRejected: {
        Qt.quit();
    }

    onAccepted: {
        Qt.quit();
    }

    onApply: {
        Qt.quit();
    }

    onYes: {
        Qt.quit();
    }

    onNo: {
        Qt.quit();
    }

    standardButtons: StandardButton.No | StandardButton.Yes | StandardButton.NoToAll
}
