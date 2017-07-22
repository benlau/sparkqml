#include <QTest>
#include <QtShell>
#include <QQmlApplicationEngine>
#include <QQmlComponent>
#include <QQuickItem>
#include "snapshottests.h"
#include "snapshot/snapshot.h"
#include "snapshot/snapshottools.h"
#include "snapshot/snapshottesting.h"

SnapshotTests::SnapshotTests(QObject *parent) : QObject(parent)
{
    auto ref = [=]() {
        QTest::qExec(this, 0, 0); // Autotest detect available test cases of a QObject by looking for "QTest::qExec" in source code
    };
    Q_UNUSED(ref);
}

void SnapshotTests::test_Snapshot()
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

void SnapshotTests::test_Snapshot_name()
{
    Snapshot snapshot = Snapshot::createFromQTest();
    QCOMPARE(snapshot.name(), QString("test_Snapshot_name"));
}

void SnapshotTests::test_Snapshot_diff()
{
    QString text1 = "A\nB\nC";
    QString text2 = "A\nD\nC";

    QString result = SnapshotTools::diff(text1, text2);

    qDebug().noquote() << result;
}

void SnapshotTests::test_SnapshotTesting_saveSnapshots()
{
    SnapshotTesting::saveSnapshots();
}

void SnapshotTests::test_Snapshot_compare()
{
    QQmlApplicationEngine engine;

    QUrl url = QUrl::fromLocalFile(QtShell::realpath_strip(SRCDIR, "sample/snapshot/Container.qml"));

    QQmlComponent component(&engine,url);
    QQuickItem *childItem = qobject_cast<QQuickItem*>(component.create());
    QVERIFY(childItem);

    Snapshot snapshot = Snapshot::createFromQTest();

    snapshot.capture(childItem);

    qDebug().noquote() << snapshot.snapshot();

    QVERIFY(snapshot.compare());

}
