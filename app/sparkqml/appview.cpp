#include <QtCore>
#include <QCoreApplication>
#include <QFAppDispatcher>
#include <QQmlContext>
#include <QDir>
#include <QtShell>
#include <QDesktopServices>
#include "qmlengine.h"
#include "appview.h"

AppView::AppView(QObject *parent) : QObject(parent)
{
}

void AppView::start()
{
    Q_ASSERT(!m_mockupFolder.isEmpty());

    m_mockupActor.setFolder(m_mockupFolder);
    m_mockupActor.init();
    m_mockupActor.createProject();

    QStringList importPathList = m_engine.importPathList();

    importPathList << "qrc:///";
    m_engine.addImportPath("qrc:///");

    QString importPathFile = m_importPathFile;

    if (!QFile::exists(importPathFile)) {
        importPathFile = QString();
    }

    if (m_importPathFile.isEmpty() &&
        !m_source.isEmpty()) {
        importPathFile = QmlEngine::searchImportPathFile(QtShell::dirname(QUrl(m_source).path()));
    }

    if (!importPathFile.isEmpty()) {
        importPathList.append(QmlEngine::readImportPathFile(importPathFile));
    }

    importPathList << m_mockupFolder;

    m_engine.setImportPathList(importPathList);

    m_engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    QFAppDispatcher* dispatcher = QFAppDispatcher::instance(&m_engine);
    connect(dispatcher,SIGNAL(dispatched(QString,QJSValue)),
            this,SLOT(onDispatched(QString,QJSValue)));

    dispatcher->dispatch("startApp");

    if (!m_source.isEmpty()) {
        loadSource();
    }
}

QString AppView::source() const
{
    return m_source;
}

void AppView::setSource(const QString &source)
{
    m_source = source;
}

void AppView::loadSource()
{    
    QFAppDispatcher* dispatcher = QFAppDispatcher::instance(&m_engine);

    QVariantMap message;
    message["source"] = m_source;

    dispatcher->dispatch("load", message);
}

QQmlApplicationEngine *AppView::engine()
{
    return &m_engine;
}

void AppView::onDispatched(QString type, QJSValue message)
{
    Q_UNUSED(message);
    if (type == "openMockupProject") {
        QDesktopServices::openUrl(QUrl::fromLocalFile(m_mockupFolder + "/mockup.qmlproject"));
    }
}

QString AppView::importPathFile() const
{
    return m_importPathFile;
}

void AppView::setImportPathFile(const QString &importPathFile)
{
    m_importPathFile = importPathFile;
}

QString AppView::mockupFolder() const
{
    return m_mockupFolder;
}

void AppView::setMockupFolder(const QString &mockupFolder)
{
    m_mockupFolder = mockupFolder;
}
