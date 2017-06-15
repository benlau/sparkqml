#include <QMetaProperty>
#include "dehydrator.h"

static QVariantMap dehydrator(QObject* source);

static auto dehydratorFunction = [](const QStringList& ignoreList) {

    return [=](QObject* source) {
        QVariantMap dest;
        const QMetaObject* meta = source->metaObject();

        for (int i = 0 ; i < meta->propertyCount(); i++) {
            const QMetaProperty property = meta->property(i);
            const char* name = property.name();
            QString stringName = name;

            if (ignoreList.indexOf(stringName) >= 0) {
                continue;
            }

            QVariant value = source->property(name);

            if (value.canConvert<QObject*>()) {
                QObject* object = value.value<QObject*>();
                if (!object) {
                    continue;
                }
                value = dehydrator(object);
            }
            dest[stringName] = value;
        }

        QObjectList children = source->children();
        QVariantList childrenDataList;
        for (int i = 0 ; i < children.size() ; i++) {
            QObject* child = children[i];
            childrenDataList << dehydrator(child);
        }

        if (childrenDataList.size() > 0) {
            dest["$children"] = childrenDataList;
        }

        return dest;
    };
};

static auto dehydrateQObject = dehydratorFunction(QStringList() << "parent");

/// Default dehydrator function
static QVariantMap dehydrator(QObject* source) {
    QVariantMap ret;

    ret = dehydrateQObject(source);

    return ret;
}


QUIKit::Dehydrator::Dehydrator()
{

}


QVariantMap QUIKit::Dehydrator::dehydrate(QObject *object)
{
    return dehydrator(object);
}
