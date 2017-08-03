#include <QTest>
#include <QtShell>
#include <QQmlApplicationEngine>
#include <QQmlComponent>
#include <QQuickItem>
#include <QJsonDocument>
#include <QQuickItemGrabResult>
#include <QQuickWindow>
#include "automator.h"
#include "snapshottests.h"
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
    {
        QString text = QtShell::cat(":/SnapshotTesting/config/snapshot-config.json");

        QJsonParseError error;
        QJsonDocument doc = QJsonDocument::fromJson(text.toUtf8(),&error);
        Q_UNUSED(doc);

        QVERIFY(error.error == QJsonParseError::NoError);
    }
}

void SnapshotTests::test_Snapshot_diff()
{
    QString text1 = "A\nB\nC";
    QString text2 = "A\nD\nC";

    QString result = SnapshotTesting::diff(text1, text2);

    qDebug().noquote() << result;
}

void SnapshotTests::test_SnapshotTesting_saveSnapshots()
{
    SnapshotTesting::saveSnapshots();
}

void SnapshotTests::test_Snapshot_matchStoredSnapshot()
{
    QFETCH(QString, input);

    QString fileName = QtShell::basename(input);

    QQmlApplicationEngine engine;
    QUrl url = QUrl::fromLocalFile(input);

    QQmlComponent component(&engine,url);
    QQuickItem *childItem = qobject_cast<QQuickItem*>(component.create());
    QVERIFY(childItem);

    QString name = QString("%1_%2").arg(QTest::currentTestFunction()).arg(fileName);

    QString text = SnapshotTesting::capture(childItem);
    text.replace(QUrl::fromLocalFile(QString(SRCDIR)).toString(), "");
    text.replace(QString(SRCDIR), "");

    QVERIFY(SnapshotTesting::matchStoredSnapshot(name, text));
}

void SnapshotTests::test_Snapshot_matchStoredSnapshot_data()
{
    QTest::addColumn<QString>("input");

    QStringList files = QtShell::find(QtShell::realpath_strip(SRCDIR, "sample/snapshot"), "*.qml");

    foreach (QString file, files) {
        QTest::newRow(file.toUtf8().constData()) << file;
    }
}

void SnapshotTests::test_Snapshot_matchStoredSnapshot_expandAll()
{
    QFETCH(QString, input);

    QString fileName = QtShell::basename(input);

    QQmlApplicationEngine engine;

    QUrl url = QUrl::fromLocalFile(input);

    QQmlComponent component(&engine,url);
    QQuickItem *childItem = qobject_cast<QQuickItem*>(component.create());
    QVERIFY(childItem);

    SnapshotTesting::Options options;
    options.expandAll = true;
    QString name = QString("%1_%2").arg(QTest::currentTestFunction()).arg(fileName);

    QString text = SnapshotTesting::capture(childItem, options);
    text.replace(QUrl::fromLocalFile(QString(SRCDIR)).toString(), "");
    text.replace(QString(SRCDIR), "");

    QVERIFY(SnapshotTesting::matchStoredSnapshot(name, text));
}

void SnapshotTests::test_Snapshot_matchStoredSnapshot_expandAll_data()
{
    QTest::addColumn<QString>("input");

    QStringList files = QtShell::find(QtShell::realpath_strip(SRCDIR, "sample/snapshot"), "*.qml");

    foreach (QString file, files) {
        QTest::newRow(file.toUtf8().constData()) << file;
    }
}
