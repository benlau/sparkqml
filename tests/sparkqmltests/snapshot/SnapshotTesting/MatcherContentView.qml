import QtQuick 2.0
import QtQuick.Controls 1.4

Item {
    id: component
    implicitWidth: 640
    implicitHeight: 480

    property string diff: ""
    property string originalVersion: ""
    property string currentVersion: ""

    TabView {
        anchors.fill: parent

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


}
