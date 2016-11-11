#include <QtQml>
#include <QtConcurrent>
#include <QtShell>
#include <QCMainThreadRunner>
#include "qmlengine.h"

static QString _searchImportPathFile(QString path)
{
    QString result;

    QDir dir(path);

    while (!dir.isRoot()) {
        QString file = dir.absolutePath() + "/qmlimport.path";

        QFileInfo info(file);
        if (info.exists()) {
            result = file;
            break;
        }

        dir.cdUp();
    }

    return result;
}

static QStringList _readImportPathFile(const QString &path)
{
    qDebug().noquote() << "Read QML_IMPORT_PATH from" << path;
    QFile file(path);
    QStringList result;

    if (!file.open(QIODevice::ReadOnly)) {
        return result;
    }

    QString content = file.readAll();
    QStringList lines = content.split("\n");
    foreach (QString line , lines) {
        if (!line.isEmpty()) {
            result << line;
        }
    }

    return result;
}

QmlEngine::QmlEngine(QObject *parent) : QObject(parent)
{
    m_engine = 0;
}

QQmlEngine *QmlEngine::engine() const
{
    return m_engine;
}

void QmlEngine::setEngine(QQmlEngine *engine)
{
    m_engine = engine;
    if (!m_engine.isNull()) {
        setPreImportPathList(engine->importPathList());

        connect(m_engine.data(), SIGNAL(warnings(QList<QQmlError>)),
                this , SLOT(onWarnings(QList<QQmlError>)));
    }
}

void QmlEngine::trimComponentCache()
{
    m_engine->trimComponentCache();
}

void QmlEngine::clearComponentCache()
{
    m_engine->clearComponentCache();
}

QFuture<bool> QmlEngine::scanImportPathList(const QString &folder)
{
    QPointer<QmlEngine> thiz = this;
    QString defaultImportPathFile = m_defaultImportPathFile;

    auto worker = [folder, thiz, defaultImportPathFile]() -> bool {

        // Avoid to call a static member function due
        // to a bug in GCC 4.7 used in travis
        QString file = _searchImportPathFile(folder);
        bool res = false;

        QStringList importPathList;

        if (!file.isEmpty() && file != defaultImportPathFile) {
            importPathList = _readImportPathFile(file);
        }

        if (!importPathList.isEmpty()) {
            res = true;
        }

        MAIN_THREAD {
            if (thiz.isNull() || thiz->engine() == 0) {
                return;
            }
            QStringList list = thiz->preImportPathList();
            list.append(importPathList);
            list.append(thiz->proImportPathList());
            thiz->engine()->setImportPathList(list);
        };

        return res;
    };

    return QtConcurrent::run(worker);
}

void QmlEngine::onWarnings(const QList<QQmlError> &warnings)
{
    bool changed = false;
    foreach (QQmlError err, warnings) {
        m_errorString += err.toString() + "\n";
        changed = true;
    }
    if (changed) {
        emit errorStringChanged();
    }
}

QString QmlEngine::defaultImportPathFile() const
{
    return m_defaultImportPathFile;
}

void QmlEngine::setDefaultImportPathFile(const QString &defaultImportPathFile)
{
    m_defaultImportPathFile = defaultImportPathFile;
}

QStringList QmlEngine::proImportPathList() const
{
    return m_proImportPathList;
}

void QmlEngine::setProImportPathList(const QStringList &proImportPathList)
{
    m_proImportPathList = proImportPathList;
}

QStringList QmlEngine::preImportPathList() const
{
    return m_preImportPathList;
}

void QmlEngine::setPreImportPathList(const QStringList &preImportPathList)
{
    m_preImportPathList = preImportPathList;
}

QString QmlEngine::errorString() const
{
    return m_errorString;
}

void QmlEngine::setErrorString(const QString &errorMessage)
{
    m_errorString = errorMessage;
    emit errorStringChanged();
}

QString QmlEngine::searchImportPathFile(QString path)
{
    return _searchImportPathFile(path);
}

QStringList QmlEngine::readImportPathFile(const QString &path)
{
    return _readImportPathFile(path);
}

