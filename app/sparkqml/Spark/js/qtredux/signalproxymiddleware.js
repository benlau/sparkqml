
function capitalizeFirstLetter(string) {
    return string.charAt(0).toUpperCase() + string.slice(1);
}

function _isSignal(object, prop) {

    var signalName = "on" + capitalizeFirstLetter(prop);

    return typeof object[prop] === "function" &&
           typeof object[signalName] === "object";
}

function createSignalProxyMiddleware(proxy) {

    var actionTable = {}

    for (var i in proxy) {
        if (_isSignal(proxy, i)) {
            actionTable[i] = true;
        }
    }

    return function () {

        return function(next) {

            return function (action) {
                next(action);
                if (actionTable.hasOwnProperty(action.type)) {
                    proxy[action.type].apply(this, action.arguments);
                }
            }

        }

    }
}
