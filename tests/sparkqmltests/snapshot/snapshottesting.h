#pragma once

#include <QString>
#include <QVariantMap>
#include <snapshot/snapshottestingoptions.h>

namespace SnapshotTesting {

    void setSnapshotsFile(const QString& file);

    QString snapshotsFile();

    QVariantMap loadStoredSnapshots();

    void saveSnapshots();

    void setSnapshot(const QString& name , const QString& content);

    void setInteractiveEnabled(bool value);

    bool interactiveEnabled();

    void setIgnoreAll(bool value);

    bool ignoreAll();

    QString capture(QObject* object, Options options = Options());

    bool matchStoredSnapshot(const QString& name, const QString& snapshot);
}
