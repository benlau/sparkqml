import QtQuick 2.4
import FontAwesome 1.0
import Spark.constants 1.0
import "../components"

Button {
    id: component
    height: 24
    implicitWidth: textMetrics.boundingRect.width + 8

    property alias text: textItem.text

    property bool last: false

    Rectangle {
        anchors.fill: parent
        opacity: 0.1
        color: "black"
        visible: component.pressed
    }

    Text {
        id: textItem
        height: 24
        font.pixelSize: 14
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        anchors.centerIn: parent
        width: parent.width
        elide: Text.ElideRight
        color: Constants.black87
    }

    TextMetrics {
        id: textMetrics
        font.pixelSize: textItem.font.pixelSize
        font.family: textItem.font.family
        text: textItem.text
    }


}
