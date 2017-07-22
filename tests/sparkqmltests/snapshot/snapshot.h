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

    static Snapshot createFromQTest();

    QString name() const;

    void setName(const QString &name);

private:
    QString m_snapshot;
    QString m_name;
};

#endif // SNAPSHOT_H
