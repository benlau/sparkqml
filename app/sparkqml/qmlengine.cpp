#include <QtQml>
#include <QtConcurrent>
#include <QtShell>
#include <QCMainThreadRunner>
#include "qmlengine.h"

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

QFuture<bool> QmlEngine::scanImportPathList(const QString &qmlFile)
{
    QPointer<QmlEngine> thiz = this;

    auto worker = [qmlFile, thiz]() -> bool {
        QString path = QtShell::dirname(QUrl(qmlFile).path());
        QString file = QmlEngine::searchImportPathFile(path);
        if (file.isEmpty()) {
            return false;
        }

        QStringList importPathList = QmlEngine::readImportPathFile(file);
        if (importPathList.isEmpty()) {
            return false;
        }

        bool res = false;

        MAIN_THREAD {
            if (thiz.isNull() || thiz->engine() == 0) {
                return;
            }
            QStringList list = thiz->preImportPathList();
            list.append(importPathList);
            list.append(thiz->proImportPathList());
            thiz->engine()->setImportPathList(list);
            res = true;
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

QStringList QmlEngine::readImportPathFile(const QString &path)
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

