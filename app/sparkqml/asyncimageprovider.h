#ifndef ASYNCIMAGEPROVIDER_H
#define ASYNCIMAGEPROVIDER_H

#include <QQuickAsyncImageProvider>
#include <QPointer>

class AsyncImageProvider: public QQuickAsyncImageProvider
{
public:
    AsyncImageProvider();

    QQuickImageResponse *requestImageResponse(const QString &id, const QSize &requestedSize);
};

#endif // ASYNCIMAGEPROVIDER_H
