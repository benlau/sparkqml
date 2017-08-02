#pragma once

#include <QString>
#include <QVariantMap>

namespace SnapshotTesting {

    void setSnapshotFiles(const QString& file);

    QString snapshotFiles();

    QVariantMap loadSnapshots();

    void saveSnapshots();

    void setSnapshot(const QString& name , const QString& content);

    void setInteractiveEnabled(bool value);

    bool interactiveEnabled();

    bool setIgnoreAll(bool value);
    bool ignoreAll();
}
