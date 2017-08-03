#ifndef SNAPSHOTTESTINGADAPTER_H
#define SNAPSHOTTESTINGADAPTER_H

#include <QObject>
#include <QVariantMap>

namespace SnapshotTesting {

    class Adapter : public QObject
    {
        Q_OBJECT
        Q_PROPERTY(QString snapshotsFile READ snapshotsFile  WRITE setSnapshotsFile  NOTIFY snapshotFileChanged)
    public:
        explicit Adapter(QObject *parent = nullptr);

        QString snapshotsFile() const;

        void setSnapshotsFile(const QString &snapshotsFile);

    signals:
        void snapshotFileChanged();

    public slots:
        QString capture(QObject* object, QVariantMap options = QVariantMap());
        bool matchStoredSnapshot(QString name, QString snapshot);

    };

}

#endif // SNAPSHOTTESTINGADAPTER_H
