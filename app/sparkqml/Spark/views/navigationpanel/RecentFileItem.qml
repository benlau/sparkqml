import QtQuick 2.0

Item {
    id: component
    property string basename: ""
    property string file: ""

    height: 28

    NavigationItem {
        width: parent.width
        text: component.basename
        icon: FontAwesome.fileO
        onClicked: {
            actions.load(file);
        }

    }

}
