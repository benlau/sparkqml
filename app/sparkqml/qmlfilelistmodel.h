#ifndef QMLFILELISTMODEL_H
#define QMLFILELISTMODEL_H

#include <QObject>
#include <QSListModel>
#include <QThreadPool>

class QmlFileListModel : public QSListModel
{
    Q_OBJECT
    Q_PROPERTY(QString folder READ folder WRITE setFolder NOTIFY folderChanged)
public:
    explicit QmlFileListModel(QObject *parent = 0);
    ~QmlFileListModel();

    QString folder() const;
    void setFolder(const QString &folder);

signals:
    void folderChanged();
    void contentReady();

public slots:
private:

    class File {
    public:
        QString preview;
        QString qml;
        QString ui;
    };

    void feed();

    QString m_folder;
    QThreadPool thread;
};

#endif // QMLFILELISTMODEL_H
