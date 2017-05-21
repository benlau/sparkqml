#include <QtCore>
#include <QtShell>
#include "sparkqmlfunctions.h"

using namespace QtShell;

QVariantMap SparkQML::parseEnvFile(const QString& content)
{
    QVariantMap map;

    QStringList lines = content.split("\n");
    for (int i = 0 ; i < lines.size();i++) {
        QString line = lines[i];
        QStringList token = line.split("=");
        if (token.size() != 2) {
            qDebug() << "Unable to parse: " << line;
            continue;
        }

        map[token[0].trimmed()] = token[1].trimmed();
    }

    return map;
}

void SparkQML::loadSparkQmlEnvFile(const QString &path)
{
    walkToRoot(path, [=](const QString& currentPath) {
        QString file = QtShell::realpath_strip(currentPath, "sparkqml.env");

        QFileInfo info(file);
        if (info.exists()) {
            QVariantMap env = parseEnvFile(cat(file));

            QMapIterator<QString, QVariant> i(env);
            qInfo() << "Load" << file;
            while (i.hasNext()) {
                i.next();
                qputenv(i.key().toLocal8Bit().constData(),i.value().toString().toLocal8Bit());
                qInfo() << i.key() << "=" << i.value().toString();
            }
            return false;
        }

        return true;
    });
}
