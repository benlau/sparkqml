import QtQuick 2.0
import QtTest 1.0

TestCase {

    function waitUntil(func, timeout) {
        if (timeout === undefined) {
            timeout = 1000;
        }

        var elapsed = 0;
        while (!func()) {
            if (elapsed > timeout) {
                return false;
            }
            wait(10);
            elapsed+=10;
        }

        return true;
    }

}
