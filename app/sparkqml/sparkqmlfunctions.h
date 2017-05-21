#ifndef SPARKQMLFUNCTIONS_H
#define SPARKQMLFUNCTIONS_H

#include <QString>
#include <QDir>
#include <QVariantMap>

namespace SparkQML {

    template <typename Predicate>
    inline void walkToRoot(const QString& path, Predicate predicate) {
        QDir dir(path);

        while (!dir.isRoot()) {

            if (!predicate(dir.absolutePath())) {
                break;
            }
            dir.cdUp();
        }
    }

    QVariantMap parseEnvFile(const QString& content);

    void loadSparkQmlEnvFile(const QString &path);
}

#endif // SPARKQMLFUNCTIONS_H
