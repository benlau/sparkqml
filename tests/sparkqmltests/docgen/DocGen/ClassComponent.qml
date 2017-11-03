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
            implicitWidth: nameMetrics.tightBoundingRect(component.name).width + component.padding * 2 + 50
            implicitHeight: nameMetrics.tightBoundingRect(component.name).height + component.padding * 2

            Text {
                id: nameText

                text: component.name
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                anchors.fill: parent
            }
        }

        Rectangle {
            height: 1
            width: parent.width
            color: "black"
        }

        Column {
            id: propertiesColumn

            Repeater {
                model: component.properties
                delegate: Item {

                    implicitWidth: attributeMetrics.tightBoundingRect(modelData).width + component.padding * 2
                    implicitHeight: attributeMetrics.tightBoundingRect(modelData).height + component.padding * 2

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
