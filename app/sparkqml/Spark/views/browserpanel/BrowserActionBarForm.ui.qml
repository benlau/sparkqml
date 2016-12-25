import QtQuick 2.4
import QtQuick.Controls 2.0
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.3
import FontAwesome 1.0

Card {
    width: 400
    height: 36
    color: "#f9f9f9"
    property alias pathViewer: pathViewer
    property alias closeButton: closeButton

    RowLayout {
        id: rowLayout1
        spacing: 8
        anchors.fill: parent

        ActionButton {
            id: closeButton
            icon: FontAwesome.close
        }

        PathViewer {
            id: pathViewer
            Layout.fillWidth: true
            Layout.fillHeight: true
        }

        ActionButton {
            id: actionButton1
            visible: false
            Layout.fillHeight: false
            Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
            icon: FontAwesome.plus
        }

        ActionButton {
            id: actionButton2
            visible: false
            Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
            icon: FontAwesome.minus
        }


    }
}
