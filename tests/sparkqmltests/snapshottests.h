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

    void test_Snapshot_name();

    void test_Snapshot_diff();
};

#endif // SNAPSHOTTESTS_H
