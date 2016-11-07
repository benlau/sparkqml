TEMPLATE = app

QT += qml quick
CONFIG += c++11

QT += quick qml multimedia

include(sparkqml.pri)

SOURCES += main.cpp

ROOTDIR = $$PWD/../..

# Default rules for deployment.
include(deployment.pri)
include(vendor/vendor.pri)

DISTFILES +=     qpm.json \
    ../../README.md

# For Qt Creator <= 4.1
QML_IMPORT_PATH += $$ROOTDIR/mockup/designer
