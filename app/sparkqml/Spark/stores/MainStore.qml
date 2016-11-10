pragma Singleton
import QtQuick 2.0

QtObject {

    property string source;

    // The folder path of source
    property string folder;

    /// The file name of loaded source
    property string fileName: ""

    /// Available states of the loaded component
    property var states: new Array;

    property string errorString: ""

    property string selectedState: "";

    property string title : "Untitled";
}
