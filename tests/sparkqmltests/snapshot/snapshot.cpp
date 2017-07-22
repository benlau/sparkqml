#include <QFileInfo>
#include <QtShell>
#include <QTest>
#include <functional>
#include <math.h>
#include <private/qqmldata_p.h>
#include <private/qqmlcontext_p.h>
#include "snapshot.h"

static QStringList ignoreList;

static QStringList knownComponentList;

static QMap<QString, QVariantMap> defaultValueMap;

static QVariantMap dehydrate(QObject* source) {

    auto itemType = [=](QObject* object) {
      const QMetaObject* meta = object->metaObject();
      QString res;

      while (knownComponentList.indexOf(res) < 0 && meta != 0) {
          res = meta->className();
          meta = meta->superClass();
      }

      return res;
    };

    auto itemName = [=](QObject* object) {
        QString result = "Item";

        if (object == source) {
            return result;
        }

        QQmlData *ddata = QQmlData::get(object, false);

        if (ddata && ddata->context) {
            QUrl fileUrl = ddata->context->url();
            qDebug() << fileUrl;

            if (!fileUrl.isEmpty()) {
                QString path = QtShell::realpath_strip(fileUrl.toString());
                QFileInfo info(path);

                result = info.completeBaseName();
            }
        }

        return result;
    };

    std::function<QVariantMap(QObject*)> _dehydrate;

    _dehydrate = [=, &_dehydrate](QObject* object) {

        QVariantMap dest;
        const QMetaObject* meta = object->metaObject();
        QString type = itemType(object);
        QVariantMap defaultValues = defaultValueMap[type];

        for (int i = 0 ; i < meta->propertyCount(); i++) {
            const QMetaProperty property = meta->property(i);
            const char* name = property.name();
            QString stringName = name;

            if (ignoreList.indexOf(stringName) >= 0) {
                continue;
            }

            QVariant value = object->property(name);

            if (value == defaultValues[stringName]) {
                // ignore default value
                continue;
            }

            if (value.canConvert<QObject*>()) {
                QObject* subObject = value.value<QObject*>();
                if (!subObject) {
                    continue;
                }
                value = _dehydrate(subObject);
            }
            dest[stringName] = value;
        }

        QObjectList children = object->children();
        QVariantList childrenDataList;
        for (int i = 0 ; i < children.size() ; i++) {
            QObject* child = children[i];
            childrenDataList << _dehydrate(child);
        }

        if (childrenDataList.size() > 0) {
            dest["$children"] = childrenDataList;
        }

        dest["$type"] = itemType(object);
        dest["$name"] = itemName(object);
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

        res = QString("").fill(' ', indent) + res;

        return res;
    };

    std::function<QString(QVariantMap, int)> _prettyText;

    _prettyText = [=, &_prettyText](QVariantMap snapshot, int indent) {
        QStringList lines;
        lines << QString().fill(' ',indent) + snapshot["$name"].toString() + " {";

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

        QVariantList children = snapshot["$children"].toList();
        for (int i = 0 ; i < children.size() ;i++) {
            QVariantMap data = children[i].toMap();
            lines << _prettyText(data, indent + 4);
        }

        lines << QString().fill(' ',indent) +  QString("}");

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
    m_snapshot = prettyText(data);
}

Snapshot Snapshot::createFromQTest()
{
    Snapshot snapshot;
    QString name = QTest::currentTestFunction();

    if (QTest::currentDataTag()) {
        name = QString("%1_%2").arg(name).arg(QTest::currentDataTag());
    }

    snapshot.m_name = name;

    return snapshot;
}

QString Snapshot::name() const
{
    return m_name;
}

void Snapshot::setName(const QString &name)
{
    m_name = name;
}

static void init() {
    ignoreList << "parent" << "transitions" << "visibleChildren"
               << "states" << "transform" << "top" << "left" << "right" << "bottom";

    knownComponentList << "QQuickItem" << "QObject";

    QVariantMap itemDefaultValue;
    itemDefaultValue["activeFocus"] = false;
    itemDefaultValue["activeFocusOnTab"] = false;
    itemDefaultValue["antialiasing"] = false;
    itemDefaultValue["baselineOffset"] = 0;
    itemDefaultValue["clip"] = false;
    itemDefaultValue["enabled"] = true;
    itemDefaultValue["focus"] = false;
    itemDefaultValue["height"] = 0;
    itemDefaultValue["x"] = 0;
    itemDefaultValue["y"] = 0;
    itemDefaultValue["z"] = 0;
    itemDefaultValue["width"] = 0;
    itemDefaultValue["visible"] = true;
    itemDefaultValue["state"] = "";
    itemDefaultValue["smooth"] = true;
    itemDefaultValue["scale"] = 1;
    itemDefaultValue["rotation"] = 0;
    itemDefaultValue["opacity"] = 1;
    itemDefaultValue["objectName"] = "";
    itemDefaultValue["implicitHeight"] = 0;
    itemDefaultValue["implicitWidth"] = 0;

    defaultValueMap["QQuickItem"] = itemDefaultValue;
}

Q_COREAPP_STARTUP_FUNCTION(init)
