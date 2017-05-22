#ifndef QMLFILELISTMODEL_H
#define QMLFILELISTMODEL_H

#include <QObject>
#include <QSListModel>
#include <QThreadPool>

namespace QUIKit {

class QmlFileListModel : public QSListModel
{
    Q_OBJECT
    Q_PROPERTY(QString folder READ folder WRITE setFolder NOTIFY folderChanged)
    Q_PROPERTY(QStringList filters READ filters WRITE setFilters NOTIFY filtersChanged)
public:
    explicit QmlFileListModel(QObject *parent = 0);
    ~QmlFileListModel();

    QString folder() const;
    void setFolder(const QString &folder);

    QStringList filters() const;

    void setFilters(const QStringList &filters);

signals:
    void folderChanged();
    void filtersChanged();
    void contentReady();

public slots:
private:

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
    };

    void feed();

    QString m_folder;
    QStringList m_filters;
};

}

#endif // QMLFILELISTMODEL_H
