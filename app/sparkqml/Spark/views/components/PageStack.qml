import QtQuick 2.0
import QtQuick.Controls 2.0

Item {

    property var pages: new Array

    property var prevPages: new Array

    property var model: ({})

    property alias initItem: stack.initialItem

    property alias pushEnter: stack.pushEnter
    property alias pushExit: stack.pushExit

    property alias popEnter : stack.popEnter
    property alias popExit : stack.popExit

    StackView {
        id: stack
        anchors.fill: parent
    }

    function update(prev,curr) {
        // Assume to have push and pop only
        if (prev.length > curr.length) {
            stack.pop();
        } else if (prev.length < curr.length){
            var item = curr[curr.length - 1];
            stack.push(model[item.name], item.properties);
        }
    }

    onPagesChanged: {
        update(prevPages, pages);
        prevPages = pages;
    }
}
