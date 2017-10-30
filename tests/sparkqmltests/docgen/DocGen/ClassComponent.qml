import QtQuick 2.4

Rectangle {

    id: component

    implicitWidth: column.width
    implicitHeight: column.height
    color: "white"
    border.color: "black"
    border.width: 1

    property string name: ""

    property var properties: []

    property int padding: 4

    FontMetrics {
        id: nameMetrics
        font: nameText.font
    }

    FontMetrics {
        id: attributeMetrics
    }

    Column {
        id: column

        Item {
            width: nameMetrics.tightBoundingRect(component.name).width + component.padding * 2
            height: nameMetrics.tightBoundingRect(component.name).height + component.padding * 2

            Text {
                id: nameText

                text: component.name
                anchors.centerIn: parent
            }
        }

        Rectangle {
            height: 1
            width: parent.width
            color: "black"
        }

        Column {

            Repeater {
                model: component.properties
                delegate: Item {

                    width: attributeMetrics.tightBoundingRect(modelData).width + component.padding * 2
                    height: attributeMetrics.tightBoundingRect(modelData).height + component.padding * 2

                    Text {
                        anchors.fill: parent
                        anchors.margins: component.padding
                        text: modelData
                    }

                }
            }


        }



    }
}
