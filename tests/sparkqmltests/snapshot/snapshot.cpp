#include <QFileInfo>
#include <QtShell>
#include <QTest>
#include <snapshot/snapshottesting.h>
#include <snapshot/snapshottools.h>
#include <QQmlApplicationEngine>
#include <QStack>
#include <functional>
#include <math.h>
#include <private/qqmldata_p.h>
#include <private/qqmlcontext_p.h>
#include "snapshot.h"

static QStringList ignoreList;

static QStringList knownComponentList;
static QMap<QString,QString> classNameToItemNameTable;

static QMap<QString, QVariantMap> defaultValueMap;

static QVariantMap dehydrate(QObject* source) {
    QString topLevelContext;
    QStack<QString> contextStack;

    auto obtainContextName = [=](QObject *object) {
        QString result;
        QQmlData *ddata = QQmlData::get(object, false);
        if (ddata && ddata->context) {
            QUrl fileUrl = ddata->context->url();

            if (!fileUrl.isEmpty()) {
                QString path = QtShell::realpath_strip(fileUrl.toString());
                QFileInfo info(path);

                result = info.completeBaseName();
            }
        }
        return result;
    };

    topLevelContext = obtainContextName(source);
    contextStack.push(topLevelContext);

    auto lastContext = [=]() {
        if (contextStack.size() == 0) {
            return QString("");
        }
        return contextStack.last();
    };

    auto obtainDefaultValuesMap = [=](QObject* object) {
        const QMetaObject* meta = object->metaObject();
        QVariantMap result;

        while (meta != 0) {
            QString className = meta->className();
            if (defaultValueMap.contains(className)) {
                QVariantMap map = defaultValueMap[className];
                QStringList keys = map.keys();
                foreach (QString key, keys) {
                    result[key] = map[key];
                }
            }

            meta = meta->superClass();
        }

        return result;
    };

    /// Obtain the class name of QObject which is known to the system
    auto obtainClassName = [=](QObject* object) {
      const QMetaObject* meta = object->metaObject();
      QString res;

      while (!classNameToItemNameTable.contains(res) && meta != 0) {
          res = meta->className();
          meta = meta->superClass();
      }

      return res;
    };

    /// Obtain the item name in QML
    auto obtainItemName = [=,&topLevelContext](QObject* object, QString className) {
        QString result = classNameToItemNameTable[className];

        if (object == source) {
            return result;
        }

        QString contextName = obtainContextName(object);
        if (contextName != topLevelContext) {
            result = contextName;
        }

        return result;
    };

    auto _dehyrdate = [=](QObject* object) {

        QVariantMap dest;
        QVariantMap defaultValues = obtainDefaultValuesMap(object);
        const QMetaObject* meta = object->metaObject();

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
                // ignore object value
                continue;
            }
            dest[stringName] = value;
        }
        return dest;
    };

    std::function<QVariantMap(QObject*)> travel;

    travel = [=, &travel, &contextStack](QObject* object) {

        QVariantMap dest;
        QString contextName = obtainContextName(object);
        bool popOnQuit = false;

        if (lastContext() != contextName) {
            if (contextStack.size() >= 2) {
                return dest;
            }
            contextStack.push(contextName);
            popOnQuit = true;
        }

        dest = _dehyrdate(object);

        QObjectList children = object->children();
        QVariantList childrenDataList;
        for (int i = 0 ; i < children.size() ; i++) {
            QObject* child = children[i];
            QVariantMap childData = travel(child);
            if (!childData.isEmpty()) {
                childrenDataList << childData;
            }
        }

        if (childrenDataList.size() > 0) {
            dest["$children"] = childrenDataList;
        }

        dest["$type"] = obtainClassName(object);
        dest["$name"] = obtainItemName(object, dest["$type"].toString());

        if (popOnQuit) {
            contextStack.pop();
        }
        return dest;
    };

    return travel(source);
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

QString Snapshot::previousSnapshot() const
{
    return SnapshotTesting::loadSnapshots()[m_name].toString();
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

bool Snapshot::compare()
{
    QVariantMap snapshots = SnapshotTesting::loadSnapshots();

    if (!snapshots.contains(m_name)) {
        SnapshotTesting::setSnapshot(m_name, m_snapshot);
        SnapshotTesting::saveSnapshots();
        return true;
    }

    QString originalVersion = snapshots[m_name].toString();

    if (originalVersion == m_snapshot) {
        return true;
    }

    QString diff = SnapshotTools::diff(originalVersion, m_snapshot);

    QQmlApplicationEngine engine;
    engine.addImportPath("qrc:///");
    engine.load(QUrl("qrc:/SnapshotTesting/Matcher.qml"));

    QObject* dialog = engine.rootObjects()[0];
    Q_ASSERT(dialog);

    dialog->setProperty("diff", diff);
    dialog->setProperty("previousSnapshot", previousSnapshot());
    dialog->setProperty("snapshot", m_snapshot);
    dialog->setProperty("title", m_name);

    QMetaObject::invokeMethod(dialog, "open");
    QCoreApplication::exec();

    int button = dialog->property("clickedButton").value<int>();
    if (button == 0x02000000) {
        SnapshotTesting::setSnapshot(m_name, m_snapshot);
        SnapshotTesting::saveSnapshots();
        return true;
    }

    return false;
}

static void init() {
    // QQuickItem
    ignoreList << "parent" << "transitions" << "visibleChildren"
               << "states" << "transform" << "top" << "left"
               << "right" << "bottom" << "transformOrigin"
               << "data" << "verticalCenter" << "transformOriginPoint"
               << "resources" << "childrenRect" << "anchors" << "children"
               << "horizontalCenter" << "baseline";

    QString text = QtShell::cat(":/SnapshotTesting/config/snapshot-config.json");

    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(text.toUtf8(),&error);

    if (error.error != QJsonParseError::NoError) {
        qWarning() << "JSON::parse() error: "<< error.errorString();
    }

    QVariantMap map = doc.object().toVariantMap();;
    knownComponentList = map.keys();
    for (int i = 0 ; i < knownComponentList.size() ; i++) {
        QString key = knownComponentList[i];
        QVariantMap record =  map[key].toMap();
        classNameToItemNameTable[key] = record["name"].toString();
        defaultValueMap[key] = record["defaultValues"].toMap();
    }
}

Q_COREAPP_STARTUP_FUNCTION(init)
