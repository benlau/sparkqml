#ifndef SPARKQMLFUNCTIONS_H
#define SPARKQMLFUNCTIONS_H

#include <QString>
#include <QDir>
#include <QVariantMap>
#include <QtShell>

namespace SparkQML {

    template <typename Predicate>
    inline void walkToRoot(const QString& path, Predicate predicate) {
        QString absPath = QtShell::realpath_strip(path);

        if (absPath.indexOf(":") == 0) {
            // Resource path is not supported
            return;
        }

        QFileInfo info(absPath);
        if (!info.isDir()) {
            absPath = info.absolutePath();
        }

        QDir dir(absPath);

        while (!dir.isRoot()) {

            if (!predicate(dir.absolutePath())) {
                break;
            }
            if (!dir.cdUp()) {
                break;
            }
        }
    }

    QVariantMap parseEnvFile(const QString& content);

    void loadSparkQmlEnvFile(const QString &path);
}

#endif // SPARKQMLFUNCTIONS_H
