#include <QtCore>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtShell>
#include "appview.h"
#include "sparkqmlfunctions.h"

#if defined(Q_OS_MAC) || defined(Q_OS_LINUX)
#include <execinfo.h>
#include <signal.h>
#include <unistd.h>

void handleBacktrace(int sig) {
    void *array[100];
    size_t size;

    // get void*'s for all entries on the stack
    size = backtrace(array, 100);

    // print out all the frames to stderr
    fprintf(stderr, "Error: signal %d:\n", sig);
    backtrace_symbols_fd(array, size, STDERR_FILENO);
    exit(1);
}
#endif

int main(int argc, char *argv[])
{
#if defined(Q_OS_MAC) || defined(Q_OS_LINUX)
    signal(SIGSEGV, handleBacktrace);
#endif

    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    qputenv("QML_DISABLE_DISK_CACHE", "1");

    QGuiApplication app(argc, argv);
    app.setApplicationName("sparkqml");

    QStringList args = app.arguments();

    if (args.size() > 1) {
        // Setup environment variable before AppView is ready
        SparkQML::loadSparkQmlEnvFile(QtShell::dirname(args[1]));
    }

    AppView view;
    view.setMockupFolder(QStandardPaths::standardLocations(QStandardPaths::AppDataLocation)[0] + "/mockup");

    if (args.size() > 1) {
        view.setSource(QUrl::fromLocalFile(args[1]).toString());
    }

    if (args.size() > 2) {
        view.setDefaultImportPathFile(args[2]);
    }

    view.start();
    return app.exec();
}
