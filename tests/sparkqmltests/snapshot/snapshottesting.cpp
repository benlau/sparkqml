#include <QtShell>
#include <QCoreApplication>
#include "snapshottesting.h"

static QString m_snapshotFile;

void SnapshotTesting::setSnapshotFiles(const QString &file)
{
    m_snapshotFile = file;
}

QString SnapshotTesting::snapshotFiles()
{
    return m_snapshotFile;
}

static void init() {
    m_snapshotFile = QtShell::realpath_strip(QtShell::pwd() + "snapshots.json");
}

Q_COREAPP_STARTUP_FUNCTION(init)
