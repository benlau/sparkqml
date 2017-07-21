#include <QTest>
#include <QtShell>
#include <QQmlApplicationEngine>
#include <QQmlComponent>
#include <QQuickItem>
#include "snapshottests.h"
#include "snapshot/snapshot.h"

using namespace std;
#include <iostream>
#include "snapshot/dtl/Sequence.hpp"
#include "snapshot/dtl/Lcs.hpp"
#include "snapshot/dtl/variables.hpp"
#include "snapshot/dtl/functors.hpp"
#include "snapshot/dtl/Ses.hpp"
#include "snapshot/dtl/Diff.hpp"

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

void SnapshotTests::test_dfl()
{
    std::vector<string> input1, input2;

    input1.push_back("A");
    input1.push_back("B");
    input1.push_back("C");

    input2.push_back("A");
    input2.push_back("D");
    input2.push_back("C");

    dtl::Diff<string> diff(input1, input2);

    diff.onHuge();
    diff.compose();
    diff.composeUnifiedHunks();
    diff.printUnifiedFormat();
}
