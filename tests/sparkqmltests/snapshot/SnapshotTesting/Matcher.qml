import QtQuick 2.0
import QtQuick.Dialogs 1.2

Dialog {
    id: component

    title: "Snapshot Testing"
    property alias diff: content.diff
    property alias originalVersion: content.originalVersion
    property alias currentVersion: content.currentVersion

    contentItem: MatcherContentView {
        id: content
    }

    standardButtons: Dialog.Apply | Dialog.Ignore
}
