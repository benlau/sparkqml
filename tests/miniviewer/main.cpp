#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtShell>
#include "appview.h"

int main(int argc, char *argv[])
{
    qputenv("QML_DISABLE_DISK_CACHE", "1");

    QGuiApplication app(argc, argv);

#if 0
    AppView appView;
    appView.setMainProgramUrl(QUrl::fromLocalFile(QtShell::realpath_strip(SRCDIR, "main.qml")));
    appView.setMockupFolder(QtShell::pwd());

    QString source = QUrl::fromLocalFile(QtShell::realpath_strip(QString(SRCDIR), "../sparkqmltests/mockup/mockup_NavigationPanel.qml")).toString();
    appView.setSource(source);

    appView.start();

#else

    QQmlApplicationEngine engine;
    engine.addImportPath("qrc:///");
    engine.load(QUrl::fromLocalFile(QtShell::realpath_strip(SRCDIR, "main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

#endif
    return app.exec();

}
