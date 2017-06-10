#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtShell>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    engine.addImportPath("qrc:///");
    engine.load(QUrl::fromLocalFile(QtShell::realpath_strip(SRCDIR, "main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
