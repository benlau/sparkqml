#ifndef HOTLOADER_H
#define HOTLOADER_H

#include <QStringList>
#include <QUrl>
#include <QFuture>
#include <QFileSystemWatcher>
#include <functional>

/// Reload your QML application when any QML files changed.
/// Status: Under development
class HotLoader : public QObject
{
public:
    HotLoader(QObject* parent = 0);

    bool hotReloadEnabled() const;

    void setHotReloadEnabled(bool hotReloadEnabled);

    void addResourceFile(const QString& file);

    /// Run a function. The call is blocked until the function terminated.
    int run(std::function<int (void)> func);

    QUrl mappedUrl(const QString& source) const;

    QString resourceMapRoot() const;
    void setResourceMapRoot(const QString &resourceMapRoot);

    int exec();

private:
    QFuture<void> compile();

    bool m_hotReloadEnabled;

    QString m_resourceMapRoot;
    QStringList m_resourceFiles;
    QStringList m_compiledResourceFiles;

    QFileSystemWatcher* watcher;
};

#endif // HOTLOADER_H
