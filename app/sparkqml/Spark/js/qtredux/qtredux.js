.pragma library
Qt.include("./signalproxymiddleware.js")

function shallowDiff(v1, v2) {
    if (v1 === v2) {
        return undefined;
    }

    if (Array.isArray(v2)) {
        // It won't compare each element in an array
        return v2;
    } else if (typeof v1 === "object" && typeof v2 === "object") {
        var res = {};
        for (var k in v2) {
            var d = shallowDiff(v1[k], v2[k]);
            if (d !== undefined) {
                res[k] = d;
            }
        }
        return res;
    } else {
        return v2;
    }
}
