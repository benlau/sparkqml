#ifndef SNAPSHOT_H
#define SNAPSHOT_H

#include <QQuickItem>
#include <QObject>

class Snapshot
{
public:
    Snapshot();

    /// The current version of snapshot
    QString snapshotText() const;

    void setSnapshotText(const QString &snapshotText);

    /// The previous version of snapshot
    QString previousSnapshotText() const;

    void capture(QObject* object);

    static Snapshot createFromQTest();

    QString name() const;

    void setName(const QString &name);

    bool matchStoredSnapshot();

    bool captureVisibleItemOnly() const;

    void setCaptureVisibleItemOnly(bool captureVisibleItemOnly);

    bool captureScreenshotEnabled() const;
    void setCaptureScreenshotEnabled(bool captureScreenshotEnabled);

    QImage screenshot() const;

    bool expandAll() const;
    void setExpandAll(bool expandAll);

private:
    QString m_snapshotText;
    QString m_name;
    bool m_captureVisibleItemOnly;
    bool m_captureScreenshotEnabled;
    bool m_expandAll;
    QImage m_screenshot;
};

#endif // SNAPSHOT_H
