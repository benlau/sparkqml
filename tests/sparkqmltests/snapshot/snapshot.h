#ifndef SNAPSHOT_H
#define SNAPSHOT_H

#include <QQuickItem>
#include <QObject>

class Snapshot
{
public:
    Snapshot();

    /// The current version of snapshot
    QString snapshot() const;

    /// The previous version of snapshot
    QString previousSnapshot() const;

    void capture(QObject* object);

    static Snapshot createFromQTest();

    QString name() const;

    void setName(const QString &name);

    bool compare();

private:
    QString m_snapshot;
    QString m_name;
};

#endif // SNAPSHOT_H
