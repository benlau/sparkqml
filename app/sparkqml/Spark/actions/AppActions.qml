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

    signal setState(string state)

    signal refresh()

    signal openDrawer()

    signal scaleToFit()

    signal openMockupProject()

    signal copyToClipboard()

    signal askToSaveFile()

    signal closeErrorPanel()
}
