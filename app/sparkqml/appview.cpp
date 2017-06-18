#include <QtCore>
#include <QCoreApplication>
#include <QFAppDispatcher>
#include <QQmlContext>
#include <QDir>
#include <QtShell>
#include <QDesktopServices>
#include "qmlengine.h"
#include "asyncimageprovider.h"
#include "appview.h"

AppView::AppView(QObject *parent) : QObject(parent)
{
    m_mainProgramUrl = QUrl(QStringLiteral("qrc:/main.qml"));
}

void AppView::start()
{
    Q_ASSERT(!m_mockupFolder.isEmpty());

    m_mockupActor.setFolder(m_mockupFolder);
    m_mockupActor.init();
    m_mockupActor.createProject();

    m_engine.addImportPath("qrc:///");
    m_engine.addImageProvider("sparkqml", new AsyncImageProvider());

    QStringList preImportPathList;
    if (!m_defaultImportPathFile.isEmpty() &&
        QFile::exists(m_defaultImportPathFile)) {
        qDebug().noquote() << "Default qmlimport.path: " << m_defaultImportPathFile;
        preImportPathList = QmlEngine::readImportPathFile(m_defaultImportPathFile);
    }
    m_engine.load(m_mainProgramUrl);

    QmlEngine* qmlEngine = qobject_cast<QmlEngine*>(QFAppDispatcher::singletonObject(&m_engine,"Spark.sys",1,0,"Engine"));
    qmlEngine->setDefaultImportPathFile(m_defaultImportPathFile);
    qmlEngine->setPreImportPathList(qmlEngine->preImportPathList() << preImportPathList);
    qmlEngine->setProImportPathList(QStringList() << m_mockupFolder);

    QObject* contentItem = m_engine.rootObjects().first()->property("contentItem").value<QObject*>();
    Q_ASSERT(contentItem);

    QObject* provider = contentItem->findChild<QObject*>("provider");
    Q_ASSERT(provider);

    connect(provider, SIGNAL(openMockupProject()), this, SLOT(openMockupProject()));

    QObject* actions = contentItem->findChild<QObject*>("actions");
    Q_ASSERT(actions);

    QMetaObject::invokeMethod(actions,"startApp");

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
    QObject* contentItem = m_engine.rootObjects().first()->property("contentItem").value<QObject*>();
    QObject* actions = contentItem->findChild<QObject*>("actions");

    QMetaObject::invokeMethod(actions,"load",Q_ARG(QVariant, m_source));
}

QUrl AppView::mainProgramUrl() const
{
    return m_mainProgramUrl;
}

void AppView::setMainProgramUrl(const QUrl &mainProgramUrl)
{
    m_mainProgramUrl = mainProgramUrl;
}

QQmlApplicationEngine *AppView::engine()
{
    return &m_engine;
}

void AppView::openMockupProject()
{
    QDesktopServices::openUrl(QUrl::fromLocalFile(m_mockupFolder + "/sparkmockup.qmlproject"));
}

QString AppView::defaultImportPathFile() const
{
    return m_defaultImportPathFile;
}

void AppView::setDefaultImportPathFile(const QString &importPathFile)
{
    m_defaultImportPathFile = importPathFile;
}

QString AppView::mockupFolder() const
{
    return m_mockupFolder;
}

void AppView::setMockupFolder(const QString &mockupFolder)
{
    m_mockupFolder = mockupFolder;
}
