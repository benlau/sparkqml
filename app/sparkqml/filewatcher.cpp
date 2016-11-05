#include <QUrl>
#include <QtQml>
#include "filewatcher.h"

FileWatcher::FileWatcher(QObject *parent) : QObject(parent)
{
    connect(&watcher, SIGNAL(fileChanged(QString)),
            this,SIGNAL(changed()));
}

QString FileWatcher::source() const
{
    return m_source;
}

void FileWatcher::setSource(const QString &source)
{
    if (!m_path.isEmpty()) {
        watcher.removePath(m_path);
    }

    m_source = source;
    emit sourceChanged();

    QUrl url(m_source);
    m_path = url.path();

    if (!m_path.isEmpty()) {
        watcher.addPath(m_path);
    }
}
