#ifndef URL_H
#define URL_H

#include <QObject>
#include <QUrl>

class Url : public QObject
{
    Q_OBJECT
public:
    explicit Url(QObject *parent = 0);

signals:

public slots:
    QString path(const QString& url);

};

#endif // URL_H
