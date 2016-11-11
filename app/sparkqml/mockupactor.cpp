#include <QtConcurrent>
#include <QtShell>
#include "mockupactor.h"

class MockupActor::Context {
public:
    QString folder;
};

MockupActor::MockupActor(QObject *parent) : QObject(parent)
{
    m_context = 0;
}

MockupActor::~MockupActor()
{
    if (m_context) {
        delete m_context;
    }
}

QString MockupActor::folder() const
{
    return m_folder;
}

void MockupActor::setFolder(const QString &folder)
{
    m_folder = folder;
}

void MockupActor::init()
{
    m_context = new Context;
    m_context->folder = m_folder;
}

QFuture<bool> MockupActor::createProject() const
{
    auto worker = [=]() {
        QDir dir(m_context->folder);
        if (dir.exists()) {
            return true;
        }

        QtShell::mkdir("-p",m_context->folder);
        return QtShell::cp("-va", ":/sparkmockupproject/*",m_context->folder);
    };

    return QtConcurrent::run(worker);
}
