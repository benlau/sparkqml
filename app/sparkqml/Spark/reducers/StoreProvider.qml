import QtQuick 2.0

QtObject {

    property string source: "";

    property string folder: ""

    /// The file name of loaded source
    property string fileName: ""

    /// Available states of the loaded component
    property var states: new Array;

    property string errorString: ""

    property string selectedState: "";

    property string title : "Untitled";

    property var views: new Array

    property string browsingFolder: ""

    signal quitApp

    signal reload
}
