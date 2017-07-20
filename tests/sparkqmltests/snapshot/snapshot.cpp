#include <QFileInfo>
#include <QtShell>
#include <math.h>
#include <private/qqmldata_p.h>
#include <private/qqmlcontext_p.h>
#include "snapshot.h"

static QStringList ignoreList;

static QVariantMap dehydrate(QObject* source) {

    auto itemName = [=](QObject* object) {
        QString result = "Item";

        if (object == source) {
            return result;
        }

        QQmlData *ddata = QQmlData::get(object, false);

        if (ddata && ddata->outerContext) {
            QUrl fileUrl = ddata->outerContext->url();

            if (!fileUrl.isEmpty()) {
                QString path = QtShell::realpath_strip(fileUrl.toString());
                QFileInfo info(path);

                result = info.completeBaseName();
            }
        }

        return result;
    };

    auto _dehydrate = [=](QObject* source) {

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
                value = dehydrate(object);
            }
            dest[stringName] = value;
        }

        QObjectList children = source->children();
        QVariantList childrenDataList;
        for (int i = 0 ; i < children.size() ; i++) {
            QObject* child = children[i];
            childrenDataList << dehydrate(child);
        }

        if (childrenDataList.size() > 0) {
            dest["$children"] = childrenDataList;
        }

        dest["$type"] = itemName(source);
        return dest;
    };

    return _dehydrate(source);
}

static QString prettyText(QVariantMap snapshot) {

    auto _prettyField = [=](QString field, QVariant v, int indent) {
        QString res;
        QString format = "%1: %2";
        QString quotedFormat = "%1: \"%2\"";

        if (v.type() == QVariant::Bool) {
            res = QString(format).arg(field).arg(v.toBool() ? "true" : "false");
        } else if (v.type() == QVariant::Double) {
            double dv = v.toDouble();
            double intpart;
            double fractpart = modf(dv, &intpart);
            if (fractpart != 0) {
                res = QString(format).arg(field).arg(dv,0,'f',2,'0');
            } else {
                res = QString(format).arg(field).arg(v.toInt());
            }

        } else if (v.type() == QVariant::String) {
            res = QString(quotedFormat).arg(field).arg(v.toString());
        } else if (v.type() == QVariant::Int) {
            res = QString(format).arg(field).arg(v.toInt());
        } else if (v.type() == QVariant::Color) {
            res = QString(quotedFormat).arg(field).arg(v.value<QColor>().name());
        } else {
            qDebug() << "Non-supported type" << v.typeName() << " Field :" << field;
            return QString("");
        }

        res = QString("").leftJustified(indent, ' ') + res;

        return res;
    };

    auto _prettyText = [=](QVariantMap snapshot, int indent) {
        QStringList lines;
        lines << snapshot["$type"].toString().leftJustified(indent, ' ');
        lines << QString("{").leftJustified(indent, ' ');

        QStringList keys = snapshot.keys();
        for (int i = 0 ; i < keys.size();i++) {
            QString key = keys[i];
            if (key.indexOf("$") == 0) {
                continue;
            }
            QString line = _prettyField(key, snapshot[key], indent + 4);
            if (!line.isEmpty())
                lines << line;
        }

        lines << QString("}").leftJustified(indent, ' ');

        return lines.join("\n");
    };

    return _prettyText(snapshot, 0);
}

Snapshot::Snapshot()
{

}

QString Snapshot::snapshot() const
{
    return m_snapshot;
}

void Snapshot::capture(QObject *object)
{
    QVariantMap data = dehydrate(object);
//    qDebug() << data;
    m_snapshot = prettyText(data);
}

static void init() {
    ignoreList << "parent" << "transitions" << "visibleChildren";
}

Q_COREAPP_STARTUP_FUNCTION(init)
