#include <QDebug>
#include <QtShell>
#include <QCoreApplication>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include "snapshottesting.h"

static QString m_snapshotFile;
static QVariantMap m_snapshots;
static bool m_snapshotsDirty = false;
static bool m_interactiveEnabled = true;

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

QVariantMap SnapshotTesting::loadSnapshots()
{
    if (!m_snapshots.isEmpty()) {
        return m_snapshots;
    }

    QVariantMap result;
    QString content = QtShell::cat(m_snapshotFile);

    if (content.isNull()) {
        return result;
    }

    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(content.toUtf8(),&error);

    if (error.error != QJsonParseError::NoError) {
        qWarning() << QString("SnapshotTesting::loadSnapshots: Failed to parse file: %1: %2").arg(m_snapshotFile).arg(error.errorString());
    }

    QVariantMap data = doc.object().toVariantMap();

    m_snapshots = data["content"].toMap();

    return m_snapshots;
}


void SnapshotTesting::saveSnapshots()
{
    if (m_snapshots.isEmpty()) {
        loadSnapshots();
    }

    if (!m_snapshotsDirty) {
        return;
    }

    m_snapshotsDirty = false;

    QVariantMap data;

    data["content"] = m_snapshots;

    QJsonObject object = QJsonObject::fromVariantMap(data);

    QJsonDocument doc;
    doc.setObject(object);
    QByteArray bytes = doc.toJson(QJsonDocument::Indented);

    QFile file;
    file.setFileName(m_snapshotFile);
    if (!file.open(QIODevice::WriteOnly)) {
        qWarning() << QString("SnapshotTesting::saveSnapshots: Failed to save snapshos file: %1").arg(file.errorString());
        return;
    }

    file.write(bytes);
    file.close();
}

void SnapshotTesting::setSnapshot(const QString &name, const QString &content)
{
    m_snapshots[name] = content;
    m_snapshotsDirty = true;
}

void SnapshotTesting::setInteractiveEnabled(bool value)
{
    m_interactiveEnabled = value;
}

bool SnapshotTesting::interactiveEnabled()
{
    return m_interactiveEnabled;
}

Q_COREAPP_STARTUP_FUNCTION(init)
