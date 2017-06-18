#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtShell>
#include "appview.h"

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
    qputenv("QML_DISABLE_DISK_CACHE", "1");
#if defined(Q_OS_MAC) || defined(Q_OS_LINUX)
    signal(SIGSEGV, handleBacktrace);
#endif
    QGuiApplication app(argc, argv);

#if 1
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
