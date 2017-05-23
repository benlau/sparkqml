INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD
QT += concurrent

HEADERS += \
    $$PWD/qmlfilelistmodel.h \
    $$PWD/qmlshell.h

SOURCES += \
    $$PWD/qmlfilelistmodel.cpp \
    $$PWD/quikittypes.cpp \
    $$PWD/qmlshell.cpp

RESOURCES += \
    $$PWD/quikit.qrc

QML_IMPORT_PATH += $$PWD
