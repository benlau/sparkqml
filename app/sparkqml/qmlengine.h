#ifndef QMLENGINE_H
#define QMLENGINE_H

#include <QObject>
#include <QQmlEngine>
#include <QPointer>

// Wrapper of QQmlEngine

class QmlEngine : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString errorString READ errorString WRITE setErrorString NOTIFY errorStringChanged)
public:
    explicit QmlEngine(QObject *parent = 0);

    QQmlEngine *engine() const;

    void setEngine(QQmlEngine *engine);

    QString errorString() const;

    void setErrorString(const QString &errorString);

    static QString searchImportPathFile(QString path);
    static QStringList readImportPathFile(const QString &path);

signals:
    void errorStringChanged();

public slots:

    void trimComponentCache();

    void clearComponentCache();

private slots:
    void onWarnings(const QList<QQmlError> &warnings);

private:
    QPointer<QQmlEngine> m_engine;
    QString m_errorString;
};

#endif // QMLENGINE_H
