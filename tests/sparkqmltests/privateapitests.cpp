#include <QtTest>
#include <QtQml/private/qqmlmetatype_p.h>
#include <QtQml/private/qhashedstring_p.h>
#include <QFAppDispatcher>
#include "privateapitests.h"

PrivateApiTests::PrivateApiTests(QObject *parent) : QObject(parent)
{
    // This function do nothing but could make Qt Creator Autotests plugin recognize QuickTests
    auto ref =[=]() {
        QTest::qExec(this, 0, 0);
    };
    Q_UNUSED(ref);
}

void PrivateApiTests::test_QQmlMetaType()
{
    QQmlEngine engine;

    // Force to load QtQuick
    QFAppDispatcher::instance(&engine);

    // That could only show loaded QML types
    foreach (const QQmlType *ty, QQmlMetaType::qmlTypes()) {
        qDebug() << ty->typeName() << ty->module();
    }
}
