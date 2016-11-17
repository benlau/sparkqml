pragma Singleton
import QtQuick 2.0
import QuickFlux 1.0

ActionCreator {

    // Call it when the initialization is finished.
    // Now, it is able to start and show the application
    signal startApp

    // Call it to quit the application
    signal quitApp

    signal load(string source)

    // Set the current state
    signal setSelectedState(string state)

    signal moveSelectedState(string direction)

    // Set the available states of current viewing component
    signal setAvailableStates(var states)

    signal reload()

    signal openDrawer()

    signal scaleToFit()

    signal resizeToFit()

    signal openMockupProject()

    signal copyToClipboard()

    signal copyToFile(string target)

    signal askToSaveFile()

    signal closeErrorPanel()

    signal setErrorString(string errorString)

    signal browse()

    signal pop()
}
