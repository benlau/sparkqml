import QtQuick 2.4

ComponentCardForm {
    id: component
    mouseArea.onClicked: {
        actions.pop();
        actions.load(component.source);
    }
}
