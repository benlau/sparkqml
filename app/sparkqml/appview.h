#pragma once

#include <QObject>
#include <QQmlApplicationEngine>
#include <QJSValue>
#include "mockupactor.h"

/* AppView is the bridge between Application(C++) and View(QML).
 */

class AppView : public QObject
{
    Q_OBJECT
public:
    explicit AppView(QObject *parent = 0);

    void start();

    QQmlApplicationEngine* engine();

    QString mockupFolder() const;
    void setMockupFolder(const QString &mockupFolder);

    QString importPathFile() const;
    void setImportPathFile(const QString &importPathFile);

    QString source() const;
    void setSource(const QString &source);

signals:

public slots:

private slots:
    void onDispatched(QString type, QJSValue message);

private:
    void loadSource();

    QQmlApplicationEngine m_engine;

    MockupActor m_mockupActor;

    QString m_importPathFile;
    QString m_mockupFolder;
    QString m_source;
};
