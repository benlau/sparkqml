#include <QtShell>
#include <QQmlApplicationEngine>
#include <QTest>
#include <Automator>
#include <QFutureWatcher>
#include "qmlengine.h"
#include "mockupactor.h"
#include "tests.h"

template <typename T>
inline T waitForFinished(QFuture<T> future) {
    if (future.isFinished()) {
        return future.result();
    }

    QFutureWatcher<T> watcher;
    watcher.setFuture(future);

    QEventLoop loop;
    QObject::connect(&watcher, SIGNAL(finished()), &loop, SLOT(quit()));

    loop.exec();
    return future.result();
}

Tests::Tests(QObject *parent) : QObject(parent)
{

}

void Tests::MockupActor_createProject()
{
    QString folder = QtShell::pwd() + "/mockup";

    if (QFileInfo::exists(folder)) {
        QtShell::rm("-rf", folder);
    }

    MockupActor actor;
    actor.setFolder(folder);
    actor.init();

    QFuture<bool> future = actor.createProject();
    future.waitForFinished();
    QVERIFY(future.result());
    QVERIFY(QFileInfo::exists(folder));

    QStringList files = QtShell::find(folder);
    QVERIFY(files.count() == 3);
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
    waitForFinished(future);

    QVERIFY(future.result());

    QCOMPARE(importPathList.size() + 2, engine.importPathList().size());

    QStringList engineImportPathList = engine.importPathList();
    QVERIFY(engineImportPathList[engineImportPathList.size() - 2] == QtShell::pwd());
    QVERIFY(engine.importPathList().last() == "qrc:///");

}
