#ifndef FILEWATCHER_H
#define FILEWATCHER_H

#include <QObject>
#include <QFileSystemWatcher>

class FileWatcher : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString source READ source WRITE setSource NOTIFY sourceChanged)
public:
    explicit FileWatcher(QObject *parent = 0);

    QString source() const;
    void setSource(const QString &source);

signals:
    void sourceChanged();

    void changed();

public slots:

private:
    QString m_source;
    QString m_path;
    QFileSystemWatcher watcher;
};

#endif // FILEWATCHER_H
