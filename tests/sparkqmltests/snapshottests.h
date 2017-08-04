#ifndef SNAPSHOTTESTS_H
#define SNAPSHOTTESTS_H

#include <QObject>

class SnapshotTests : public QObject
{
    Q_OBJECT
public:
    explicit SnapshotTests(QObject *parent = nullptr);

signals:

public slots:

private slots:
    void test_Snapshot();

    void test_Snapshot_diff();

    void test_SnapshotTesting_saveSnapshots();

    void test_SnapshotTesting_capture_QObject();

    void test_SnapshotTesting_matchStoredSnapshot();
    void test_SnapshotTesting_matchStoredSnapshot_data();

    void test_SnapshotTesting_matchStoredSnapshot_expandAll();
    void test_SnapshotTesting_matchStoredSnapshot_expandAll_data();

    void test_SnapshotTesting_matchStoredSnapshot_hideId();
    void test_SnapshotTesting_matchStoredSnapshot_hideId_data();

private:
    void scanSamples();
};


#endif // SNAPSHOTTESTS_H
