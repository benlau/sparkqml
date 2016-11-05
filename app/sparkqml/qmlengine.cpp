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
}

void QmlEngine::trimComponentCache()
{
    m_engine->trimComponentCache();
}

void QmlEngine::clearComponentCache()
{
    m_engine->clearComponentCache();
}

