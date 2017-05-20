#include <QtQml>
#include "qmlfilelistmodel.h"

using namespace QUIKit;

static void registerTypes() {
    qmlRegisterType<QmlFileListModel>("QUIKit", 1, 0, "QmlFileListModel");
}

Q_COREAPP_STARTUP_FUNCTION(registerTypes)
