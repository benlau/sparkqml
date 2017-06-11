#pragma once

#include <QObject>

namespace QUIKit {

/// Wrapper of QtShell
class QmlShell : public QObject
{
    Q_OBJECT
public:
    explicit QmlShell(QObject *parent = 0);

signals:

public slots:
    QString basename(const QString& path);
    QString dirname(const QString& path);
    QStringList find(const QString& path);

    QStringList find(const QString& path, const QStringList& filter);

    QString cat(const QString& path);

    QString realpathStrip(const QString& path);
};

};
