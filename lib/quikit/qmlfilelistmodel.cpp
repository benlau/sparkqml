#include <QPointer>
#include <QtConcurrent>
#include <QtShell>
#include <QSDiffRunner>
#include <asyncfuture.h>
#include <aconcurrent.h>
#include "qmlfilelistmodel.h"

template <typename T, typename Functor>
static QList<T> filter(const QList<T>& list, Functor func ) {
    QList<T> result;
    foreach (T item , list) {
        if (func(item)) {
            result << item;
        }
    }
    return result;
}

template <typename O, typename T, typename F>
QList<O> map(const QList<T> &input, F f) {
    QList<O> result;

    for (int i = 0 ; i < input.size() ; i++) {
        result << f(input[i]);
    }

    return result;
}

template <typename T, typename F>
QList<T> uniq(const QList<T>& input, F f) {
    QMap<QVariant,bool> indexTable;
    QList<T> res = filter<T>(input, [&indexTable, f](const T& item) {
        QVariant k = f(item);
        bool res = true;
        if (indexTable.contains(k)) {
            res = false;
        } else  {
            indexTable[k] = true;
        }
        return res;
    });
    return res;
}

namespace QUIKit {

QmlFileListModel::QmlFileListModel(QObject *parent) : QSListModel(parent)
{
}

QmlFileListModel::~QmlFileListModel()
{
}

QString QmlFileListModel::folder() const
{
    return m_folder;
}

void QmlFileListModel::setFolder(const QString &folder)
{
    QString value = QUrl(folder).path();
    if (m_folder == value) {
        return;
    }
    m_folder = value;
    folderChanged();
    feed();
}

void QmlFileListModel::feed()
{
    QPointer<QmlFileListModel> thiz = this;
    QString folder = m_folder;
    QStringList filters = m_filters;

    auto runFilters = [=](QList<QFileInfo> input) {
        QList<QFileInfo> list;

        if (filters.size() == 0) {
            return input;
        }

        for (int i = 0 ; i < filters.size(); i++) {
            QString filter = filters[i];

            int j = 0;
            while (j < input.size()) {
                QFileInfo info = input[j];
                QString fileName = info.baseName();
                QRegExp rx(filter, Qt::CaseInsensitive, QRegExp::Wildcard);

                if (fileName.indexOf(rx) >= 0) {
                    list << info;
                    input.removeAt(j);
                } else {
                    j++;
                }
            }
        }

        return list;
    };

    auto worker = [thiz, folder, runFilters]() {
        QDir dir(folder);
        QList<QFileInfo> fileInfoList = dir.entryInfoList();

        QMap<QString, bool> index;

        /// Search only qml file and build index
        fileInfoList = filter<QFileInfo>(fileInfoList, [&index](const QFileInfo& info) {
            bool res = true;
            index[info.fileName()] = true;
            if (info.fileName() == "." ||
                info.fileName() == ".." ||
                !QRegExp(".*qml").exactMatch(info.fileName())) {
                res = false;
            }
            return res;
        });

        fileInfoList = runFilters(fileInfoList);

        // Convert QList<QFileInfo> to QList<File>
        QList<File> res = map<File>(fileInfoList, [index](const QFileInfo& info) {
            File file;

            if (QRegExp(".*ui.qml").exactMatch(info.fileName())) {
                file.ui = info.absoluteFilePath();
                QString qmlFileName = info.baseName().replace(QRegExp("Form$"),"") + ".qml";
                if (index.contains(qmlFileName)) {
                    file.qml = info.absolutePath() + "/" + qmlFileName;
                }
            } else {
                file.qml = info.absoluteFilePath();
                QString formFileName = info.baseName() + "Form.ui.qml";
                if (index.contains(formFileName)) {
                    file.ui = info.absolutePath() + "/" + formFileName;
                }
            }

            if (!file.ui.isEmpty()) {
                // Prefer to show UI Form rather than qml file.
                file.preview = QUrl::fromLocalFile(file.ui).toString();
            } else {
                file.preview = QUrl::fromLocalFile(file.qml).toString();
            }

            if (!file.qml.isEmpty()) {
                // file.source prefer to load QML file (e.g preview transition effect)
                file.source = QUrl::fromLocalFile(file.qml).toString();
            } else {
                file.source = QUrl::fromLocalFile(file.ui).toString();
            }

            file.ui = QtShell::basename(file.ui);
            file.qml = QtShell::basename(file.qml);
            return file;
        });

        res = uniq(res, [](const File& item) {
            return item.preview;
        });

        return res;
    };

    auto cleanup = [=](QList<File> res) {
        if (thiz->folder() != folder ) {
            return;
        }

        QVariantList curr = map<QVariant>(res, [](const File& item ) {
            QVariantMap map;
            map["source"] = item.source;
            map["preview"] = item.preview;
            map["ui"] = item.ui;
            map["qml"] = item.qml;
            return (QVariant) map;
        });

        QSDiffRunner runner;
        runner.setKeyField("preview");
        QSPatchSet patch = runner.compare(thiz->storage(), curr);
        runner.patch(thiz.data(), patch);

        emit contentReady();
    };

    auto f = QtConcurrent::run(worker);

    AConcurrent::debounce(this,"feed", f, [=]() {
        cleanup(f.result());
    });
}

void QmlFileListModel::setFilters(const QStringList &filters)
{
    m_filters = filters;
    feed();
    emit filtersChanged();
}

QStringList QmlFileListModel::filters() const
{
    return m_filters;
}

} // End of Namespace
