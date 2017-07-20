#include <QtShell>
#include <QQmlApplicationEngine>
#include <QTest>
#include <Automator>
#include <QQmlEngine>
#include <QQmlInfo>
#include <QFutureWatcher>
#include <appview.h>
#include <qmlfilelistmodel.h>
#include "qmlengine.h"
#include "mockupactor.h"
#include "tests.h"
#include "sparkqmlfunctions.h"
#include "dehydrator.h"
#include "aconcurrent.h"
#include "snapshot/snapshot.h"

using namespace SparkQML;
using namespace QUIKit;
using namespace AConcurrent;

Tests::Tests(QObject *parent) : QObject(parent)
{

    auto ref = [=]() {
        QTest::qExec(this, 0, 0); // Autotest detect available test cases of a QObject by looking for "QTest::qExec" in source code
    };
    Q_UNUSED(ref);
}

void Tests::mockupLoadingTests()
{
    QStringList files = QtShell::find(QString(SRCDIR) + "mockup", QStringList() << "*.qml");

    files << "qrc:/main.qml";

    QQmlApplicationEngine engine;
    engine.addImportPath("qrc:///");
    foreach (QString file , files) {
        QQmlComponent comp(&engine);

        if (file.indexOf("qrc") == 0) {
            comp.loadUrl(QUrl(file));
        } else {
            comp.loadUrl(QUrl::fromLocalFile(file));
        }

        if (comp.isError()) {
            qDebug() << QString("%1 : Load Failed. Reason :  %2").arg(file).arg(comp.errorString());
        }
        QVERIFY(!comp.isError());
        qDebug() << QString("%1 : Passed").arg(file);
    }
}

void Tests::MockupActor_createProject()
{
    QString folder = QtShell::pwd() + "/mockup_project";

    if (QFileInfo::exists(folder)) {
        QtShell::rm("-rf", folder);
    }

    MockupActor actor;
    actor.setFolder(folder);
    actor.init();

    QFuture<bool> future = actor.createProject();
    AConcurrent::await(future);
    QVERIFY(future.result());
    QVERIFY(QFileInfo::exists(folder));

    QStringList files = QtShell::find(folder);
    qDebug() << files;
    QCOMPARE(files.count(), 3);
}

void Tests::QmlEngine_scanImagePath()
{
    QQmlApplicationEngine engine;
    QStringList importPathList = engine.importPathList();
    QmlEngine qmlEngine;

    qmlEngine.setEngine(&engine);
    QVERIFY(engine.importPathList() == qmlEngine.preImportPathList());
    qmlEngine.setProImportPathList(QStringList() << "qrc:///");

    QtShell::mkdir("-p", "fakeproject/App");
    QtShell::touch("fakeproject/App/test.qml");
    QFile importPathFile("fakeproject/qmlimport.path");
    QVERIFY(importPathFile.open(QIODevice::WriteOnly));
    importPathFile.write(QtShell::pwd().toLocal8Bit());
    importPathFile.close();

    QFuture<bool> future = qmlEngine.scanImportPathList("fakeproject/App/test.qml");
    AConcurrent::await(future);

    QVERIFY(future.result());

    QCOMPARE(importPathList.size() + 2, engine.importPathList().size());

    QStringList engineImportPathList = engine.importPathList();
    QVERIFY(engineImportPathList[engineImportPathList.size() - 2] == QtShell::pwd());
    QVERIFY(engine.importPathList().last() == "qrc:///");

}

void Tests::QmlEngine_scanImagePath_withDefaultFile()
{
    QQmlApplicationEngine engine;
    QStringList importPathList = engine.importPathList();
    QmlEngine qmlEngine;

    qmlEngine.setEngine(&engine);
    QVERIFY(engine.importPathList() == qmlEngine.preImportPathList());
    qmlEngine.setProImportPathList(QStringList() << "qrc:///");

    QtShell::mkdir("-p", "fakeproject/App");
    QtShell::touch("fakeproject/App/test.qml");
    QFile importPathFile("fakeproject/qmlimport.path");
    QVERIFY(importPathFile.open(QIODevice::WriteOnly));
    importPathFile.write(QtShell::pwd().toLocal8Bit());
    importPathFile.close();

    qmlEngine.setDefaultImportPathFile(QFileInfo("fakeproject/qmlimport.path").absoluteFilePath());
    QFuture<bool> future = qmlEngine.scanImportPathList("fakeproject/App/test.qml");
    AConcurrent::await(future);

    QVERIFY(!future.result());

    QCOMPARE(importPathList.size() + 1, engine.importPathList().size());

    QStringList engineImportPathList = engine.importPathList();
    QVERIFY(engineImportPathList.last() == "qrc:///");
}

void Tests::test_QmlFileListModel_folder()
{
    QmlFileListModel model;
    QString folder = QtShell::pwd() + "/QmlFileListModel_test";
    QtShell::mkdir("-p", folder);

    QtShell::touch(folder + "/Sample1.qml");
    QtShell::touch(folder + "/Sample2.qml");
    QtShell::touch(folder + "/Sample2Form.ui.qml");
    QtShell::touch(folder + "/README.md");

    QVERIFY(model.count() == 0);
    model.setFolder(folder);
    QVERIFY(Automator::waitUntilSignal(&model,SIGNAL(contentReady())));
    QCOMPARE(model.count(), 2);

    // Test filters
    {
        model.setFilters(QStringList() << "Sample1*");
        QVERIFY(Automator::waitUntilSignal(&model,SIGNAL(contentReady())));
        QCOMPARE(model.count(), 1);

        model.setFilters(QStringList() << "*");

        QVERIFY(Automator::waitUntilSignal(&model,SIGNAL(contentReady())));
        QCOMPARE(model.count(), 2);
        QVERIFY(model.get(0)["qml"].toString() == "Sample1.qml");

        model.setFilters(QStringList() << "Sample2" << "Sample1");

        QVERIFY(Automator::waitUntilSignal(&model,SIGNAL(contentReady())));
        QCOMPARE(model.count(), 2);
        QVERIFY(model.get(0)["qml"].toString() == "Sample2.qml");

    }
}

void Tests::test_QmlFileListModel_process()
{
    QmlFileListModel model;

    QStringList input;
    input << "Sample1.qml" << "Sample2.qml" << "Sample2Form.ui.qml" << "README.md";

    QVERIFY(model.count() == 0);
    model.process(input);

    QCOMPARE(model.count(), 2);
}

void Tests::test_QmlFileListModel_options()
{
    QmlFileListModel model;
    QVariantMap options;
    QVariantMap properties;

    properties["width"] = 100;
    options["Sample2"] = properties;

    QStringList input;
    input << "Sample1.qml" << "Sample2.qml" << "Sample2Form.ui.qml" << "README.md";

    QVERIFY(model.count() == 0);
    model.setOptions(options);
    model.process(input);

    QCOMPARE(model.count(), 2);

    QVariantMap item = model.get(1);

    QCOMPARE(item["qml"].toString(), QString("Sample2.qml"));
    QCOMPARE(item["properties"].toMap()["width"].toInt(), 100);

}

void Tests::test_parseEnvFile()
{
    QString content = "A=123\nB= 456 ";

    QVariantMap env = parseEnvFile(content);

    QCOMPARE(env.size(), 2);
    QVERIFY(env["A"] == "123");
    QVERIFY(env["B"] == "456");
}

void Tests::test_SparkQML_walkToRoot()
{
    int count = 0;

    auto counter = [&](const QString &path) {
        Q_UNUSED(path);
        count++;
        return true;
    };

    SparkQML::walkToRoot("qrc:///1/2", counter);
    QCOMPARE(count, 0);

    SparkQML::walkToRoot("/1/2/3", counter);
    QCOMPARE(count, 1);

    count = 0;
    SparkQML::walkToRoot(QtShell::pwd(), counter);
    QVERIFY(count > 0);

    count = 0;
    SparkQML::walkToRoot(QUrl::fromLocalFile(QtShell::realpath_strip(QtShell::pwd(), "tests.cpp")).toString() , counter);
    QVERIFY(count > 2);
}

void Tests::test_Dehydrator()
{
    {
        // Basic

        QObject* parent = new QObject();
        parent->setObjectName("parent");

        QObject* child = new QObject(parent);
        child->setObjectName("child");
        Dehydrator dehydrator;

        QVariantMap data = dehydrator.dehydrate(child);

        QCOMPARE(data["objectName"].toString(), QString("child"));
        QCOMPARE(data.size(), 1);

        data = dehydrator.dehydrate(parent);
        QCOMPARE(data["objectName"].toString(), QString("parent"));
        QCOMPARE(data.size(), 2);
        QVariantList list = data["$children"].toList();
        QCOMPARE(list.size(), 1);
        QCOMPARE(list[0].toMap()["objectName"].toString() , QString("child"));
    }
}

void Tests::test_Snapshot()
{
    QQmlApplicationEngine engine;

    QUrl url = QUrl::fromLocalFile(QtShell::realpath_strip(SRCDIR, "sample/snapshot/Container.qml"));

    QQmlComponent component(&engine,url);
    QQuickItem *childItem = qobject_cast<QQuickItem*>(component.create());
    QVERIFY(childItem);

    Snapshot snapshot;

    snapshot.capture(childItem);

    qDebug().noquote() << snapshot.snapshot();
}

#if 0
void Tests::test_AppView()
{
    AppView *appView = new AppView();

    appView->setMockupFolder(QtShell::pwd());

    appView->setSource(QUrl::fromLocalFile(QtShell::realpath_strip(SRCDIR, "mockup/mockup_BoardLayout.qml")).toString());

    qDebug() << "start";
    appView->start();

    QEventLoop loop;
    QTimer::singleShot(5000, [&loop]() {
        loop.quit();;
    });
    loop.exec();
    qDebug() << "finished";

    delete appView;
}
#endif
