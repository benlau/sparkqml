#include <QtCore>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "appview.h"

int main(int argc, char *argv[])
{
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);
    app.setApplicationName("sparkqml");
    Q_UNUSED(app);

    QStringList args = app.arguments();

    AppView view;
    view.setMockupFolder(QStandardPaths::standardLocations(QStandardPaths::AppDataLocation)[0] + "/mockup");

    if (args.size() > 1) {
        view.setSource(QUrl::fromLocalFile(args[1]).toString());
    }

    if (args.size() > 2) {
        view.setImportPathFile(args[2]);
    }

    view.start();
    return app.exec();
}
