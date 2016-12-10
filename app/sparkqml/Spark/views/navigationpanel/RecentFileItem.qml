import QtQuick 2.0
import FontAwesome 1.0

Item {
    id: component
    property string displayName: ""
    property string file: ""

    height: 28

    NavigationItem {
        width: parent.width
        text: component.displayName
        icon: FontAwesome.fileO
        onClicked: {
            actions.load(file);
        }

    }

}
