#include "url.h"

Url::Url(QObject *parent) : QObject(parent)
{

}

QString Url::path(const QString &url)
{
    QUrl u(url);
    return u.path();
}
