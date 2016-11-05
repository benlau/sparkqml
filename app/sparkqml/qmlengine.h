#ifndef QMLENGINE_H
#define QMLENGINE_H

#include <QObject>
#include <QQmlEngine>
#include <QPointer>

// Wrapper of QQmlEngine

class QmlEngine : public QObject
{
    Q_OBJECT
public:
    explicit QmlEngine(QObject *parent = 0);

    QQmlEngine *engine() const;

    void setEngine(QQmlEngine *engine);

signals:

public slots:

    void trimComponentCache();

    void clearComponentCache();

private:
    QPointer<QQmlEngine> m_engine;

};

#endif // QMLENGINE_H
