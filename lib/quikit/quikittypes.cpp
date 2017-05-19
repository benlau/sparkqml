#include <QtQml>
#include "qmlfilelistmodel.h"

static void registerTypes() {
    qmlRegisterType<QmlFileListModel>("QUIKit", 1, 0, "QmlFileListModel");
}

Q_COREAPP_STARTUP_FUNCTION(registerTypes)
