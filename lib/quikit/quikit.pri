INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD
QT += concurrent

HEADERS += \
    $$PWD/qmlfilelistmodel.h \
    $$PWD/qmlshell.h \
    $$PWD/hotloader.h

SOURCES += \
    $$PWD/qmlfilelistmodel.cpp \
    $$PWD/quikittypes.cpp \
    $$PWD/qmlshell.cpp \
    $$PWD/hotloader.cpp

RESOURCES += \
    $$PWD/quikit.qrc

QML_IMPORT_PATH += $$PWD

# Experimental hot loader
HOT_LOADER_HOST_BINS = $$[QT_HOST_BINS]
DEFINES += HOT_LOADER_HOST_BINS=\\\"$$HOT_LOADER_HOST_BINS\\\"
