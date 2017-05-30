#ifndef QMLFILELISTMODEL_H
#define QMLFILELISTMODEL_H

#include <QObject>
#include <QSListModel>
#include <QThreadPool>
#include <QFuture>

namespace QUIKit {

class QmlFileListModel : public QSListModel
{
    Q_OBJECT
    Q_PROPERTY(QString folder READ folder WRITE setFolder NOTIFY folderChanged)
    Q_PROPERTY(QStringList filters READ filters WRITE setFilters NOTIFY filtersChanged)
    Q_PROPERTY(QVariantMap options READ options WRITE setOptions NOTIFY optionsChanged)
public:
    explicit QmlFileListModel(QObject *parent = 0);
    ~QmlFileListModel();

    QString folder() const;
    void setFolder(const QString &folder);

    QStringList filters() const;

    void setFilters(const QStringList &filters);

    QVariantMap options() const;
    void setOptions(const QVariantMap &options);

signals:
    void folderChanged();
    void filtersChanged();
    void contentReady();
    void optionsChanged();

public slots:
    void process(const QStringList& input);

public:
    class File {
    public:
        /// Preview file URL
        QString preview;

        /// The source file URL to be loaded
        QString source;

        /// The implementation file
        QString qml;

        /// The UI form file
        QString ui;

        /// The name of the file (without .qml)
        QString title;

        /// The options to be passed to the QML file for preview.
        QVariantMap options;
    };

private:
    void feed();
    void setContent(const QList<File>& files);

    QString m_folder;
    QStringList m_filters;

    QVariantMap m_options;
};

}

#endif // QMLFILELISTMODEL_H
