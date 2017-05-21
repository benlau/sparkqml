#ifndef SPARKQMLFUNCTIONS_H
#define SPARKQMLFUNCTIONS_H

#include <QString>
#include <QDir>

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

}

#endif // SPARKQMLFUNCTIONS_H
