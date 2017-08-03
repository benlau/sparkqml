import QtQuick 2.6
import QtQuick.Controls 1.4

Item {
    id: contentView
    implicitWidth: 640
    implicitHeight: 480

    property string diff: ""
    property string originalVersion: ""
    property string currentVersion: ""

    property url screenshot: ""

    TabView {
        anchors.fill: parent
        anchors.bottomMargin: 40

        Tab {
            title: "Diff"
            Item {
                TextArea {
                    anchors.fill: parent
                    text: diff
                }
            }
        }

        Tab {
            title: "Original Version"
            Item {
                TextArea {
                    anchors.fill: parent
                    text: originalVersion
                }
            }
        }

        Tab {
            title: "Current Version"
            Item {
                TextArea {
                    anchors.fill: parent
                    text: currentVersion
                }
            }
        }
    }

    Text {
        anchors.bottom: parent.bottom
        width: parent.width
        height: 40
        verticalAlignment: Text.AlignVCenter

        leftPadding: 10
        rightPadding: 10
        text: qsTr("New snapshot does not match the stored snapshot. Inspect your code and press \"Yes\" to update the changes, or press \"No\" to reject.")

        wrapMode: Text.WordWrap

    }


}
