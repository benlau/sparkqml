.pragma library
Qt.include("./signalproxymiddleware.js")
Qt.include("./syncmiddleware.js")

function shallowDiff(v1, v2) {
    if (v1 === v2) {
        return undefined;
    }

    if (typeof v1 === "object" &&
        typeof v2 === "object" &&
        !Array.isArray(v2)) { // Don't compare array element.

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

function assign(dest, src) {
    if (dest === undefined ||
        src === undefined) {
        return dest;
    }

    for (var i in src) {
        if (!dest.hasOwnProperty(i)) {
            continue;
        }

        if (typeof src[i] === "object" && !Array.isArray(src[i])){
            assign(dest[i], src[i])
        } else {
            dest[i] = src[i];
        }
    }

    return dest;
}
