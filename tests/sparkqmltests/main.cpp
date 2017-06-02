#include <QString>
#include <QtTest>

#include <TestRunner>
#include <QtQuickTest/quicktest.h>
#include "tests.h"
#include "hotloadertests.h"

namespace AutoTestRegister {
    QUICK_TEST_MAIN(QuickTests)
}

#if defined(Q_OS_MAC) || defined(Q_OS_LINUX)
#include <execinfo.h>
#include <unistd.h>
#include <signal.h>
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

    QGuiApplication app(argc, argv);

    TestRunner runner;
    runner.addImportPath("qrc:///");
    runner.add<Tests>();
    runner.add<HotLoaderTests>();
    runner.add(QString(SRCDIR) + "/qmltests");

    bool error = runner.exec(app.arguments());

    if (!error) {
        qDebug() << "All test cases passed!";
    }

    return error;
}
