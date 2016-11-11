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
    qmltests/tst_MainStoreWorker.qml \
    sample/Rect.qml \
    mockup/mockup_ComponentViewer.qml

HEADERS +=     tests.h


