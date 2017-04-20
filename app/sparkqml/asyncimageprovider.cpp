#include <QTimer>
#include <QtCore>
#include <asyncfuture.h>
#include <QUrlQuery>
#include <QtConcurrent>
#include "asyncimageprovider.h"

using namespace AsyncFuture;

class AsyncImageResponse : public QQuickImageResponse {
public:

    QQuickTextureFactory *textureFactory() const
    {
        return m_texture;
    }

    void setResult(QImage image) {
        m_texture = QQuickTextureFactory::textureFactoryForImage(image);
        emit finished();
    }

    QQuickTextureFactory *m_texture;

};

AsyncImageProvider::AsyncImageProvider()
{

}

QQuickImageResponse *AsyncImageProvider::requestImageResponse(const QString &id, const QSize &requestedSize)
{
    Q_UNUSED(requestedSize);
    QUrl url(id);
    QUrlQuery query(url);
    QString path = QString("/") + url.path();

    int delay = 0;

    AsyncImageResponse* response = new AsyncImageResponse();

    auto readImage = [=]() {
        QImage image;
        if (!image.load(path)) {
            qWarning() << "Failed to open file";
        }
        response->setResult(image);
    };

    if (query.hasQueryItem("delay")) {
        delay = query.queryItemValue("delay").toInt(0,10);
    }

    if (delay > 0) {
        QTimer::singleShot(delay, QCoreApplication::instance(), [=]() {
            QtConcurrent::run(readImage);
        });
    } else {
        QtConcurrent::run(readImage);
    }

    return response;
}
