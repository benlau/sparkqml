CONFIG   += c++11
QT += concurrent

SOURCES += $$PWD/appview.cpp \
    $$PWD/filewatcher.cpp \
    $$PWD/mockupactor.cpp \
    $$PWD/qmlengine.cpp \
    $$PWD/qmltypes.cpp \
    $$PWD/url.cpp \
    $$PWD/fileinfo.cpp \
    $$PWD/asyncimageprovider.cpp \
    $$PWD/clipboardwrapper.cpp \
    $$PWD/sparkqmlfunctions.cpp

RESOURCES += \    
    $$PWD/sparkqml.qrc

INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH += $$PWD

HEADERS +=     $$PWD/appview.h \
    $$PWD/filewatcher.h \
    $$PWD/mockupactor.h \
    $$PWD/qmlengine.h \
    $$PWD/url.h \
    $$PWD/fileinfo.h \
    $$PWD/asyncimageprovider.h \
    $$PWD/clipboardwrapper.h \
    $$PWD/sparkqmlfunctions.h
