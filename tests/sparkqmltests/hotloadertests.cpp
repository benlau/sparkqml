#include <QQmlApplicationEngine>
#include <QTest>
#include <Automator>
#include "hotloader.h"
#include "hotloadertests.h"

HotLoaderTests::HotLoaderTests(QObject *parent) : QObject(parent)
{
    // This function do nothing but could make Qt Creator Autotests plugin recognize QuickTests
    auto ref =[=]() {
        QTest::qExec(this, 0, 0);
    };
    Q_UNUSED(ref);

}

void HotLoaderTests::test_HotLoader_run()
{
    bool executed = false;
    bool exists = false;

    HotLoader loader;
    loader.setHotReloadEnabled(true);
    loader.addResourceFile(QString(SRCDIR) + "/hotloadertests.qrc");

    int ret = loader.run([&]() {
        executed = true;
        exists = QFile::exists(":hot-loader-dynamic-resource/hotloader/1.txt");
        return -1;
    });

    QCOMPARE(executed, true);
    QCOMPARE(exists, true);
    QCOMPARE(ret, -1);

    QCOMPARE(QFile::exists(":hot-loader-dynamic-resource/1.txt"), false); // it is already unloaded
}

void HotLoaderTests::test_HotLoader_mappedUrl()
{
    qDebug() << "test_HotLoader_mappedUrl";
    HotLoader loader;

    loader.setHotReloadEnabled(false);
    QCOMPARE(loader.mappedUrl(":/main.qml"), QUrl("qrc:/main.qml"));
    loader.setHotReloadEnabled(true);
    QCOMPARE(loader.mappedUrl(":/main.qml"), QUrl("qrc:/hot-loader-dynamic-resource/main.qml"));

}

