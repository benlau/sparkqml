QT       += testlib qml

TARGET = sparkqmltests
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

SOURCES +=     main.cpp     tests.cpp

DEFINES += SRCDIR=\\\"$$PWD/\\\"
ROOTDIR = $$PWD/../../

include(vendor/vendor.pri)
include($$ROOTDIR/app/sparkqml/sparkqml.pri)


DISTFILES +=     qpm.json \     
    DummyItem1.qml \
    sample/SyntaxError.qml \
    sample/Rect.qml \
    mockup/mockup_ComponentViewer.qml \
    qmltests/tst_ComponentViewer.qml \
    mockup/mockup_BrowserPanel.qml \
    mockup/mockup_ViewStack.qml \
    mockup/mockup_ComponetCard.qml \
    qmltests/tst_Redux.qml \
    qmltests/tst_Reducer.qml \
    qmltests/tst_Middleware.qml \
    mockup/mockup_RecentFileList.qml \
    mockup/mockup_NavigationPanel.qml \
    mockup/mockup_PathViewer.qml

HEADERS +=     tests.h


