#include <QtShell>
#include <QtQml>
#include "qmlshell.h"

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

static QObject* provider(QQmlEngine *engine, QJSEngine *scriptEngine) {
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)

    QmlShell* object = new QmlShell();
    return object;
}

static void registerTypes() {
    qmlRegisterSingletonType<QmlShell>("Shell", 1, 0, "Shell", provider);
}

Q_COREAPP_STARTUP_FUNCTION(registerTypes)
