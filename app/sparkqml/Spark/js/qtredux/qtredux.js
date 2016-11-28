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

function assign(dest, src) {
    if (dest === undefined ||
        src === undefined) {
        return dest;
    }

    for (var i in src) {
        if (!dest.hasOwnProperty(i)) {
            continue;
        }

        if (Array.isArray(src[i])) {
            dest[i] = src[i];
        } else if (typeof src[i] === "object"){
            assign(dest[i], src[i])
        } else {
            dest[i] = src[i];
        }
    }
}
