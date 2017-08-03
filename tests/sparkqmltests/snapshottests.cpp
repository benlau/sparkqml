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
    {
        QString text = QtShell::cat(":/SnapshotTesting/config/snapshot-config.json");

        QJsonParseError error;
        QJsonDocument doc = QJsonDocument::fromJson(text.toUtf8(),&error);
        Q_UNUSED(doc);

        QVERIFY(error.error == QJsonParseError::NoError);
    }

    QQmlApplicationEngine engine;

    QUrl url = QUrl::fromLocalFile(QtShell::realpath_strip(SRCDIR, "sample/snapshot/Sample1.qml"));

    QQmlComponent component(&engine,url);
    QQuickItem *childItem = qobject_cast<QQuickItem*>(component.create());
    QVERIFY(childItem);

    Snapshot snapshot;

    snapshot.capture(childItem);

    qDebug().noquote() << snapshot.snapshotText();
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
    QFETCH(QString, input);

    QQuickWindow window;
    window.setVisible(true);

    QString fileName = QtShell::basename(input);

    QQmlApplicationEngine engine;
    engine.setIncubationController(window.incubationController());

    QUrl url = QUrl::fromLocalFile(input);

    QQmlComponent component(&engine,url);
    QQuickItem *childItem = qobject_cast<QQuickItem*>(component.create());
    childItem->setParentItem(window.contentItem());
    QVERIFY(childItem);

    Snapshot snapshot;
    snapshot.setName(QString("%1_%2").arg(QTest::currentTestFunction()).arg(fileName));
    snapshot.setCaptureScreenshotEnabled(true);

    snapshot.capture(childItem);
    qDebug().noquote() << snapshot.snapshotText();

    QString text = snapshot.snapshotText();
    text.replace(QUrl::fromLocalFile(QString(SRCDIR)).toString(), "");
    text.replace(QString(SRCDIR), "");

    snapshot.setSnapshotText(text);

    QVERIFY(snapshot.compare());

    {
        QString screenshotFolder = QtShell::realpath_strip(QtShell::pwd(), "screenshots");
        QtShell::mkdir("-p", screenshotFolder);

        QString screenshotFile = QtShell::realpath_strip(screenshotFolder, QtShell::basename(input)) + ".png";

        QImage image = snapshot.screenshot();
        QVERIFY(image.save(screenshotFile));
    }

}

void SnapshotTests::test_Snapshot_compare_data()
{
    QTest::addColumn<QString>("input");

    QStringList files = QtShell::find(QtShell::realpath_strip(SRCDIR, "sample/snapshot"), "*.qml");

    foreach (QString file, files) {
        QTest::newRow(file.toUtf8().constData()) << file;
    }
}
