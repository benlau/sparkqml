#include <QtConcurrent>
#include <QtShell>
#include <QResource>
#include <asyncfuture.h>
#include "hotloader.h"

using namespace AsyncFuture;

template <typename T, typename F, typename I>
QList<T> blockingMapped(QList<I> input, F f) {
    QList<QFuture<T> > futures;
    QList<T> res;

    for (int i = 0 ; i < input.size(); i++) {
        futures << QtConcurrent::run(f, input[i]);
    }

    for (int i = 0 ; i < input.size(); i++) {
        futures[i].waitForFinished();
        res << futures[i].result();
    }

    return res;
}

template <typename T>
void waitForFinished(QList<QFuture<T> > futures) {
    for (int i = 0 ; i < futures.size(); i++) {
        futures[i].waitForFinished();
    }
}

template <typename T>
void waitForFinished(QFuture<T> future) {

    QEventLoop loop;
    while (!future.isFinished()) {
        loop.processEvents(QEventLoop::AllEvents, 10);
    }
}

static QStringList parseRcc(const QString& rccFile) {
    // It don't use XML parser to avoid unnecessary library dependence
    QString content = QtShell::cat(rccFile);
    QStringList lines = content.split("\n");
    QStringList files;

    QRegularExpression re("<file>(.*)</file>");

    foreach (QString line, lines) {
        QRegularExpressionMatch match = re.match(line);
        if (match.hasMatch()) {
            files << match.captured(1);
        }
    }

    return files;
}

static QString compileRcc(const QString& file) {
    //@TODO - handle Windows

    QString rcc = QString(HOT_LOADER_HOST_BINS) + "/rcc";
    QString output = QtShell::basename(file).replace(".qrc","") + ".rcc";
    QProcess process;
    QStringList arguments;

    arguments << "--binary" << file << "-o" << QtShell::pwd() + "/" + output;
    qDebug().noquote() << rcc << arguments.join(" ");
    process.start(rcc, arguments);
    process.setProcessChannelMode(QProcess::MergedChannels);
    process.waitForFinished();

    if (process.exitCode() != 0) {
        qWarning() << process.errorString();
    }

    return output;
}

HotLoader::HotLoader(QObject* parent) : QObject(parent),
                                        m_hotReloadEnabled(false),
                                        m_resourceMapRoot("/hot-loader-dynamic-resource"),
                                        watcher(new QFileSystemWatcher(this))
{
    connect(watcher, &QFileSystemWatcher::fileChanged,
            this, [=]() {
        qDebug() << "changed";
    });
}

bool HotLoader::hotReloadEnabled() const
{
    return m_hotReloadEnabled;
}

void HotLoader::setHotReloadEnabled(bool hotReloadEnabled)
{
    m_hotReloadEnabled = hotReloadEnabled;
}

void HotLoader::addResourceFile(const QString &file)
{
    m_resourceFiles << file;
    QStringList files = parseRcc(file);

    foreach (QString f, files) {
        qDebug() << "Monitor" << f;
        watcher->addPath(f);
    }
}

int HotLoader::run(std::function<int (void)> func)
{
    QFuture<void> future;
    if (m_hotReloadEnabled) {
        future = compile();
    }

    waitForFinished(future);

    int ret = func();

    foreach (QString rccFile, m_compiledResourceFiles) {
        QResource::unregisterResource(rccFile, m_resourceMapRoot);
    }

    return ret;
}

QUrl HotLoader::mappedUrl(const QString &source) const
{
    QUrl input(source);
    if (!input.isValid()) {
        if (source.indexOf(":") == 0) {
            input = QUrl();
            input.setPath(source.mid(1,-1));
            input.setScheme("qrc");
        } else {
            input = QUrl::fromLocalFile(source);
        }
    }

    if (!m_hotReloadEnabled) {
        return input;
    }

    QString path = input.path();
    input.setPath(m_resourceMapRoot + path);
    return input;
}

QFuture<void> HotLoader::compile()
{
    QFuture<QString> result = QtConcurrent::mapped(m_resourceFiles, compileRcc);

    return observe(result).context(this, [=]() {
        m_compiledResourceFiles = result.results();

        foreach (QString rccFile, m_compiledResourceFiles) {
            QResource::unregisterResource(rccFile, m_resourceMapRoot);
            QResource::registerResource(rccFile, m_resourceMapRoot);
        }

    }).future();
}

QString HotLoader::resourceMapRoot() const
{
    return m_resourceMapRoot;
}

void HotLoader::setResourceMapRoot(const QString &mapRoot)
{
    m_resourceMapRoot = mapRoot;
}

int HotLoader::exec()
{
    return QCoreApplication::instance()->exec();
}
