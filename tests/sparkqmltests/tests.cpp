#include <QtShell>
#include <QQmlApplicationEngine>
#include <QTest>
#include <Automator>
#include "mockupactor.h"
#include "tests.h"

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
