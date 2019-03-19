#!/bin/sh

BUILD_PATH=`pwd`/build-release
PRO=`pwd`/app/sparkqml/sparkqml.pro
PKG_PATH=`pwd`/build-release/package
SRC_PATH=`pwd`
SCRIPT_PATH=${SRC_PATH}/deployment
QML_PATH=${SRC_PATH}/app/sparkqml
APPIMAGE_PATH=${BUILD_PATH}/AppImage
BIN_PATH=${PKG_PATH}/SparkQML
QMAKE=`which qmake`

set -v
set -e

mkdir -p $BUILD_PATH
mkdir -p $APPIMAGE_PATH

cd $BUILD_PATH
$QMAKE INSTALL_ROOT=${PKG_PATH} $PRO
make
make install

linuxdeployqt.py ${BIN_PATH} \
  --qml-import Qt.labs.settings \
  --qt-base-dir `qmake -query QT_INSTALL_PREFIX` \
  --qml-scan-dir ${QML_PATH} \
  --appdir ${APPIMAGE_PATH}

rm ${APPIMAGE_PATH}/default.desktop
cp ${SCRIPT_PATH}/sparkqml.desktop ${APPIMAGE_PATH}/SparkQML.desktop

appimagetool ${APPIMAGE_PATH}

