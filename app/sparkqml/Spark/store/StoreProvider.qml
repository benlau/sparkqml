import QtQuick 2.0

Object {

    property string source: "";

    property string folder: ""

    /// The file name of loaded source
    property string fileName: ""

    /// Available states of the loaded component
    property var availableStates: new Array;

    property string errorString: ""

    property string selectedState: "";

    property string mainWindowTitle : "SparkQML";

    property bool mainWindowVisible : false;

    property var views: new Array

    property string browsingFolder: ""

    property var recentFiles: new Array

    property alias navigationPanel: navigationPanel

    Object {
        id: navigationPanel
        property var displayRecentFiles: new Array;
    }

    signal quitApp

    signal load

    signal reload

    signal openDrawer

    signal closeDrawer

    signal scaleToFit

    signal resizeToFit

    signal openMockupProject

    signal copyToClipboard

    signal copyToFile(string file)

    signal askToSaveFile

    signal forceActiveFocusOnStateListView

}
