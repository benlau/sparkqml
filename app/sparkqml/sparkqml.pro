TEMPLATE = app

QT += qml quick
CONFIG += c++11

QT += quick qml multimedia

include(sparkqml.pri)

TARGET = SparkQML
SOURCES += main.cpp

ROOTDIR = $$PWD/../..

# Default rules for deployment.
include(deployment.pri)
include(vendor/vendor.pri)
include($$ROOTDIR/lib/quikit/quikit.pri)

DISTFILES +=     qpm.json \
    ../../README.md

ICON = sparkqml.icns

linux {
    xcbplugins.path = plugins
    xcbplugins.files += $$[QT_INSTALL_DATA]/plugins/xcbglintegrations/*.so
    INSTALLS += xcbplugins
}

# For Qt Creator <= 4.1
QML_IMPORT_PATH += $$ROOTDIR/mockup/designer

