QT       += testlib qml qml-private

TARGET = sparkqmltests
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

SOURCES +=     main.cpp     tests.cpp \
    hotloadertests.cpp \
    snapshottests.cpp \
    snapshot/snapshottesting.cpp \
    snapshot/snapshottestingadapter.cpp

DEFINES += SRCDIR=\\\"$$PWD/\\\"
ROOTDIR = $$PWD/../../

include(vendor/vendor.pri)
include($$ROOTDIR/app/sparkqml/sparkqml.pri)
include($$ROOTDIR/lib/quikit/quikit.pri)

DEFINES += QUICK_TEST_SOURCE_DIR=\\\"$$PWD\\\"

DISTFILES +=     qpm.json \     
    DummyItem1.qml \
    sample/Rect.qml \
    mockup/mockup_ComponentViewer.qml \
    qmltests/tst_ComponentViewer.qml \
    mockup/mockup_BrowserPanel.qml \
    mockup/mockup_ComponetCard.qml \
    qmltests/tst_Reducer.qml \
    qmltests/tst_Middleware.qml \
    mockup/mockup_RecentFileList.qml \
    mockup/mockup_NavigationPanel.qml \
    mockup/mockup_PathViewer.qml \
    sample/rectanlges/Red100x100.qml \
    sample/rectanlges/Blue100x50.qml \
    mockup/mockup_UIGrid.qml \
    qmltests/tst_NameTag.qml \
    mockup/mockup_UIFlow.qml \
    sample/rectanlges/Green50x20.qml \
    ../../appveyor.yml \
    hotloader/1.txt \
    ../../qpm.json \
    qmltests/tst_QmlFileListModel.qml \
    qmltests/TestUtils.qml \
    mockup/mockup_BoxPackingFlow.qml \
    mockup/mockup_UIBoxPackingFlow.qml \
    qmltests/tst_BoardLayout.qml \
    mockup/mockup_BoardLayout.qml \
    sample/snapshot/Sample1.qml \
    snapshot/snapshots.json \
    sample/snapshot/Sample2.qml \
    sample/snapshot/Sample3.qml \
    qmltests/tst_SnapshotTesting.qml \
    sample/snapshot/Sample4.qml

HEADERS +=     tests.h \
    hotloadertests.h \
    snapshot/dtl/Diff.hpp \
    snapshot/dtl/Diff3.hpp \
    snapshot/dtl/dtl.hpp \
    snapshot/dtl/functors.hpp \
    snapshot/dtl/Lcs.hpp \
    snapshot/dtl/Sequence.hpp \
    snapshot/dtl/Ses.hpp \
    snapshot/dtl/variables.hpp \
    snapshottests.h \
    snapshot/snapshottesting.h \
    snapshot/snapshottestingadapter.h

RESOURCES += \
    hotloadertests.qrc \
    snapshot/snapshottesting.qrc

