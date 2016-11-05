pragma Singleton
import QtQuick 2.0

QtObject {

    property string source;

    /// The file name of loaded source
    property string fileName: ""

    property var states: new Array;

    property string selectedState: "";

    property string title : "Untitled";
}
