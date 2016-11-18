import QtQuick 2.0

QtObject {

    property var dispatch: function(){}

    function startApp() {
        dispatch({
            type: "startApp"
        })
    }

}
