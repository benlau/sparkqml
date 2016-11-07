#include <QtQml>
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

