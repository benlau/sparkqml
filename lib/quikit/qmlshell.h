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

};

};
