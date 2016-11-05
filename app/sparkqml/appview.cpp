#include <QtCore>
#include <QCoreApplication>
#include <QFAppDispatcher>
#include <QQmlContext>
#include <QDir>
#include <QtShell>
#include <QDesktopServices>
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
        importPathFile = searchImportPathFile(QtShell::dirname(QUrl(m_source).path()));
    }

    if (!importPathFile.isEmpty()) {
        importPathList.append(readImportPathFile(importPathFile));
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

QStringList AppView::readImportPathFile(const QString &path)
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

QString AppView::searchImportPathFile(QString path)
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

QQmlApplicationEngine *AppView::engine()
{
    return &m_engine;
}

void AppView::onDispatched(QString type, QJSValue message)
{
    Q_UNUSED(type);
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
