#include <QtShell>
#include <QtQml>
#include "qmlshell.h"

namespace QUIKit {

QmlShell::QmlShell(QObject *parent) : QObject(parent)
{

}

QString QmlShell::basename(const QString &path)
{
    return QtShell::basename(path);
}

QString QmlShell::dirname(const QString &path)
{
    return QtShell::dirname(path);
}

QStringList QmlShell::find(const QString &path)
{
    return QtShell::find(path);
}

QStringList QmlShell::find(const QString &path, const QStringList &filter)
{
    return QtShell::find(path, filter);
}

}
