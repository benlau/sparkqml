import QtQuick 2.4
import QtQuick.Layouts 1.3
import QtQuick.Controls 1.4
import QtQuick.Controls 2.0
import "."

Item {
    width: 640
    height: 480
    property alias mainViewer: mainViewer

    SplitView {
        anchors.fill: parent

        Item {
            Layout.fillHeight: true
            Layout.fillWidth: true

            ColumnLayout {
                spacing: 0
                anchors.fill: parent
                ToolBar {
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    Layout.maximumHeight: 24

                }

                Viewer {
                    id: mainViewer
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                }
            }
        }

        StatesPanel {
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.maximumWidth: 240

        }

    }
}
