#include <QFileInfo>
#include <QtShell>
#include <QTest>
#include <snapshot/snapshottesting.h>
#include <snapshot/snapshottools.h>
#include <snapshot/snapshottestingoptions.h>
#include <QBuffer>
#include <QJsonDocument>
#include <QJsonObject>
#include <QQmlApplicationEngine>
#include <QQuickItemGrabResult>
#include <QStack>
#include <functional>
#include <math.h>
#include "snapshot.h"

using namespace SnapshotTesting;

Snapshot::Snapshot()
{
    m_captureVisibleItemOnly = true;
    m_captureScreenshotEnabled = false;
    m_expandAll = false;
}

QString Snapshot::snapshotText() const
{
    return m_snapshotText;
}

QString Snapshot::previousSnapshotText() const
{
    return SnapshotTesting::loadStoredSnapshots()[m_name].toString();
}

void Snapshot::capture(QObject *object)
{
    SnapshotTesting::Options options;

    options.captureVisibleItemOnly = m_captureVisibleItemOnly;
    options.expandAll = m_expandAll;

    m_snapshotText = SnapshotTesting::capture(object, options);

    if (m_captureScreenshotEnabled) {
        QQuickItem* item = qobject_cast<QQuickItem*>(object);
        if (item) {
            QSharedPointer<QQuickItemGrabResult> result = item->grabToImage();
            QEventLoop loop;
            QObject::connect(result.data(), SIGNAL(ready()),&loop,SLOT(quit()));
            loop.exec();
            m_screenshot = result->image();
        }
    }
}

Snapshot Snapshot::createFromQTest()
{
    Snapshot snapshot;
    QString name = QTest::currentTestFunction();

    if (QTest::currentDataTag()) {
        name = QString("%1_%2").arg(name).arg(QTest::currentDataTag());
    }

    snapshot.m_name = name;

    return snapshot;
}

QString Snapshot::name() const
{
    return m_name;
}

void Snapshot::setName(const QString &name)
{
    m_name = name;
}

bool Snapshot::matchStoredSnapshot()
{
    QVariantMap snapshots = SnapshotTesting::loadStoredSnapshots();

    if (!snapshots.contains(m_name)) {
        SnapshotTesting::setSnapshot(m_name, m_snapshotText);
        SnapshotTesting::saveSnapshots();
        return true;
    }

    QString originalVersion = snapshots[m_name].toString();

    if (originalVersion == m_snapshotText) {
        return true;
    }

    QString diff = SnapshotTools::diff(originalVersion, m_snapshotText);

    qDebug().noquote() << "Snapshot::matchStoredSnapshot: The snapshot is different:";
    qDebug().noquote() << diff;

    if (SnapshotTesting::interactiveEnabled() && !SnapshotTesting::ignoreAll()) {
        QQmlApplicationEngine engine;
        engine.addImportPath("qrc:///");
        engine.load(QUrl("qrc:/SnapshotTesting/Matcher.qml"));

        QObject* dialog = engine.rootObjects()[0];
        Q_ASSERT(dialog);

        dialog->setProperty("diff", diff);
        dialog->setProperty("previousSnapshot", previousSnapshotText());
        dialog->setProperty("snapshot", m_snapshotText);
        dialog->setProperty("title", m_name);

        if (!m_screenshot.isNull()) {
            QByteArray ba;
            QBuffer buffer(&ba);
            buffer.open(QBuffer::WriteOnly);
            m_screenshot.save(&buffer, "PNG");
            buffer.close();
            QString base64 = QString("data:image/png;base64,") + ba.toBase64();
            QUrl source(base64);
            dialog->setProperty("screenshot", source);
        }

        QMetaObject::invokeMethod(dialog, "open");
        QCoreApplication::exec();

        int button = dialog->property("clickedButton").value<int>();
        switch (button) {
        // Use hex code to avoid the dependence to QtWidget
        case 0x00020000: // No to all
            SnapshotTesting::setIgnoreAll(true);
            break;
        case 0x00004000:
        case 0x02000000:
            SnapshotTesting::setSnapshot(m_name, m_snapshotText);
            SnapshotTesting::saveSnapshots();
            return true;
            break;
        }
    }

    return false;
}

bool Snapshot::captureVisibleItemOnly() const
{
    return m_captureVisibleItemOnly;
}

void Snapshot::setCaptureVisibleItemOnly(bool captureVisibleItemOnly)
{
    m_captureVisibleItemOnly = captureVisibleItemOnly;
}

bool Snapshot::captureScreenshotEnabled() const
{
    return m_captureScreenshotEnabled;
}

void Snapshot::setCaptureScreenshotEnabled(bool captureScreenshotEnabled)
{
    m_captureScreenshotEnabled = captureScreenshotEnabled;
}

QImage Snapshot::screenshot() const
{
    return m_screenshot;
}

bool Snapshot::expandAll() const
{
    return m_expandAll;
}

void Snapshot::setExpandAll(bool expendAll)
{
    m_expandAll = expendAll;
}

void Snapshot::setSnapshotText(const QString &snapshotText)
{
    m_snapshotText = snapshotText;
}
