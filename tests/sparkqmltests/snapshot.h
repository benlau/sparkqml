#ifndef SNAPSHOT_H
#define SNAPSHOT_H

#include <QQuickItem>
#include <QObject>

class Snapshot
{
public:
    Snapshot();

    QString snapshot() const;

    void capture(QObject* object);

private:
    QString m_snapshot;
};

#endif // SNAPSHOT_H
