#ifndef DEHYDRATOR_H
#define DEHYDRATOR_H

#include <QString>
#include <QObject>
#include <QVariantMap>

namespace QUIKit {

class Dehydrator
{
public:
    Dehydrator();

    QVariantMap dehydrate(QObject* object);
};

}

#endif // DEHYDRATOR_H
