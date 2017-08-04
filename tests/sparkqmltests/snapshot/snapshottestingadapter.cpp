#include <QCoreApplication>
#include <QQmlEngine>
#include "snapshot/snapshottesting.h"
#include "snapshot/snapshottestingadapter.h"

using namespace SnapshotTesting;

Adapter::Adapter(QObject *parent) : QObject(parent)
{

}

QString Adapter::capture(QObject *object, QVariantMap options)
{
    SnapshotTesting::Options opt;

    if (options.contains("captureVisibleItemOnly")) {
        opt.captureVisibleItemOnly = options["captureVisibleItemOnly"].toBool();
    }

    if (options.contains("expandAll")) {
        opt.expandAll = options["expandAll"].toBool();
    }

    if (options.contains("hideId")) {
        opt.hideId = options["hideId"].toBool();
    }

    return SnapshotTesting::capture(object, opt);
}

bool Adapter::matchStoredSnapshot(QString name, QString snapshot)
{
    return SnapshotTesting::matchStoredSnapshot(name, snapshot);
}

QString Adapter::snapshotsFile() const
{
    return SnapshotTesting::snapshotsFile();
}

void Adapter::setSnapshotsFile(const QString &snapshotFile)
{
    SnapshotTesting::setSnapshotsFile(snapshotFile);
    emit snapshotFileChanged();
}

static QObject *provider(QQmlEngine *engine, QJSEngine *scriptEngine) {
    Q_UNUSED(engine);
    Q_UNUSED(scriptEngine);
    
    Adapter* object = new Adapter();

    return object;
}

static void registerTypes() {
    qmlRegisterSingletonType<Adapter>("SnapshotTesting", 1, 0, "SnapshotTesting", provider);
}

Q_COREAPP_STARTUP_FUNCTION(registerTypes)
