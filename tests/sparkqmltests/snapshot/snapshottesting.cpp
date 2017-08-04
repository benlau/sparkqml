#include <QDebug>
#include <QtShell>
#include <QCoreApplication>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <QStack>
#include <QFileInfo>
#include <QQmlApplicationEngine>
#include <QQmlComponent>
#include <QQuickItem>
#include <QBuffer>
#include "snapshottesting.h"
#include <private/qqmldata_p.h>
#include <private/qqmlcontext_p.h>
#include <functional>

/* For dtl */
using namespace std;
#include <iostream>
#include <sstream>
#include <vector>
#include "snapshot/dtl/Sequence.hpp"
#include "snapshot/dtl/Lcs.hpp"
#include "snapshot/dtl/variables.hpp"
#include "snapshot/dtl/functors.hpp"
#include "snapshot/dtl/Ses.hpp"
#include "snapshot/dtl/Diff.hpp"


static QString m_snapshotFile;
static QVariantMap m_snapshots;
static bool m_snapshotsDirty = false;
static bool m_interactiveEnabled = true;
static bool m_ignoreAll = false;
static bool m_acceptAll = false;

static QStringList knownComponentList;
static QMap<QString,QString> classNameToItemNameTable;

static QMap<QString, QVariantMap> defaultValueMap;
static QMap<QString, QStringList> ignoreListMap;

static QVariantMap dehydrate(QObject* source, const SnapshotTesting::Options& options) {
    QString topLevelContextName;
    QUrl topLevelBaseUrl;
    QQmlContext* topLevelContext = qmlContext(source);
    bool captureVisibleItemOnly = options.captureVisibleItemOnly;
    bool expandAll = options.expandAll;

    auto obtainContextName = [=](QObject *object) {
        QString result;
        QQmlData *ddata = QQmlData::get(object, false);
        if (ddata && ddata->context) {
            // obtain the inner context name
            QUrl fileUrl = ddata->context->url();

            if (!fileUrl.isEmpty()) {
                QString path = QtShell::realpath_strip(fileUrl.toString());
                QFileInfo info(path);

                result = info.completeBaseName();
            }
        }
        return result;
    };

    topLevelContextName = obtainContextName(source);
    if (topLevelContext) {
        topLevelBaseUrl = topLevelContext->baseUrl();
    }

    auto obtainId = [=](QObject* object) -> QString {
        if (!topLevelContext) {
            return "";
        }
        QString res = topLevelContext->nameForObject(object);
        if (res.isEmpty()) {
            QQmlContext* context = qmlContext(object);
            if (context) {
                res = context->nameForObject(object);
            }
        }
        return res;
    };

    auto obtainClassName = [=](QObject* object) {
        const QMetaObject* meta = object->metaObject();
        return meta->className();
    };

    /// Obtain the class name of QObject which is known to the system
    auto obtainKnownClassName = [=](QObject* object) {
      const QMetaObject* meta = object->metaObject();
      QString res;

      while (!classNameToItemNameTable.contains(res) && meta != 0) {
          res = meta->className();
          meta = meta->superClass();
      }

      return res;
    };

    /// Obtain the item name in QML
    auto obtainItemName = [=,&topLevelContextName](QObject* object) {
        QString result;
        QString className = obtainClassName(object);
        if (className.indexOf("QQuick") == 0) {
            result = className.replace("QQuick", "");
        }

        if (result.isNull()) {
            QString knownClassName = obtainKnownClassName(object);

            result = classNameToItemNameTable[knownClassName];
        }

        if (object == source) {
            return result;
        }

        if (!expandAll) {
            QString contextName = obtainContextName(object);
            if (contextName != topLevelContextName && contextName != "") {
                result = contextName;
            }
        }

        return result;
    };

    auto obtainDynamicGeneratedDefaultValuesMap = [=](QObject* object) {
        static QMap<QString, QVariantMap> autoDefaultValueMap;

        QString className = obtainClassName(object);
        if (autoDefaultValueMap.contains(className)) {
            return autoDefaultValueMap[className];
        }

        QVariantMap res;
        if (className.indexOf("QQuick") != 0) {
            return res;
        }

        QString itemName = obtainItemName(object);

        QQmlApplicationEngine engine;

        QString qml  = QString("import QtQuick 2.4\n import QtQuick.Controls 2.0\n %1 {}").arg(itemName);

        QQmlComponent comp (&engine);
        comp.setData(qml.toUtf8(),QUrl());
        QObject* holder = comp.create();

        if (holder) {
            const QMetaObject* meta = holder->metaObject();

            for (int i = 0 ; i < meta->propertyCount(); i++) {

                const QMetaProperty property = meta->property(i);
                const char* name = property.name();

                QVariant value = holder->property(name);
                res[name] = value;
            }
            delete holder;
        }

        autoDefaultValueMap[className] = res;
        return res;
    };


    auto obtainDefaultValuesMap = [=](QObject* object) {
        const QMetaObject* meta = object->metaObject();
        QVariantMap result = obtainDynamicGeneratedDefaultValuesMap(object);

        QStringList classes;

        while (meta != 0) {
            QString className = meta->className();
            classes << className;
            meta = meta->superClass();
        }

        while (classes.size() > 0) {
            QString className = classes.takeLast();
            if (defaultValueMap.contains(className)) {
                QVariantMap map = defaultValueMap[className];
                QStringList keys = map.keys();
                foreach (QString key, keys) {
                    result[key] = map[key];
                }
            }
        }

        return result;
    };

    auto obtainIgnoreList = [=](QObject* object) {
        const QMetaObject* meta = object->metaObject();
        QStringList result;
        while (meta != 0) {
            QString className = meta->className();
            if (ignoreListMap.contains(className)) {
                QStringList list = ignoreListMap[className];
                result.append(list);
            }

            meta = meta->superClass();
        }

        return result;
    };

    auto _dehyrdate = [=](QObject* object) {

        QVariantMap dest;
        QVariantMap defaultValues = obtainDefaultValuesMap(object);
        QStringList ignoreList = obtainIgnoreList(object);

        const QMetaObject* meta = object->metaObject();

        QString id = obtainId(object);
        if (!id.isNull() && !options.hideId) {
            dest["id"] = id;
        }

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

    auto inherited = [=](QObject *object, QString className) {
        bool res = false;

        const QMetaObject *metaObject = object->metaObject();

        while (metaObject) {
            if (metaObject->className() == className) {
                res = true;
                break;
            }
            metaObject = metaObject->superClass();
        }

        return res;
    };

    auto isVisible = [=](QObject* object) {
        QQuickItem* item = qobject_cast<QQuickItem*>(object);

        if (!item) {
            return false;
        }

        if (item->opacity() == 0 ||
            !item->isVisible()) {
            return false;
        }

        return true;
    };

    auto _allowTravel = [=](QObject* object) {
        if (!captureVisibleItemOnly) {
            return true;
        }

        return isVisible(object);
    };

    std::function<QVariantMap(QObject*)> travel;

    travel = [=, &travel](QObject* object) {
        if (!_allowTravel(object)) {
            return QVariantMap();
        }

        QVariantMap dest;

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

        QString className = obtainClassName(object);
        if (className == "QQuickRepeater") {
            int count = object->property("count").toInt();
            for (int i = 0 ; i < count; i++) {
                QQuickItem* child;
                QMetaObject::invokeMethod(object,"itemAt",Qt::DirectConnection,
                                          Q_RETURN_ARG(QQuickItem*,child),
                                          Q_ARG(int,i));
                QVariantMap childData = travel(child);
                if (!childData.isEmpty()) {
                    childrenDataList << childData;
                }
            }
        } else if (inherited(object, "QQuickFlickable")) {

            QQuickItem* contentItem = object->property("contentItem").value<QQuickItem*>();

            if (contentItem) {
                QList<QQuickItem *>items = contentItem->childItems() ;

                for (int i = 0 ;  i < items.size() ; i++) {
                    QVariantMap childData = travel(items.at(i));
                    if (!childData.isEmpty()) {
                        childrenDataList << childData;
                    }
                }
            }
        }

        if (childrenDataList.size() > 0) {
            dest["$children"] = childrenDataList;
        }

        dest["$class"] = obtainKnownClassName(object);
        dest["$name"] = obtainItemName(object);

        QUrl baseUrl;
        QQmlContext *context = qmlContext(object);
        if (context) {
            baseUrl = context->baseUrl();
        }

        if (!expandAll && baseUrl != topLevelBaseUrl) {
            dest["$skip"] = true;
        }

        return dest;
    };

    if (captureVisibleItemOnly && !isVisible(source)) {
        qDebug() << "SnapshotTesting::capture(): The capture target is not visible";
    }

    return travel(source);
}

static QString prettyText(QVariantMap snapshot) {
    QStringList priorityFields;

    priorityFields << "objectName" << "x" << "y" << "width" << "height";

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
        } else if (v.type() == QVariant::Url) {
            res = QString(quotedFormat).arg(field).arg(v.toUrl().toString());
        } else if (v.type() == QVariant::Size) {
            QSize size = v.toSize();
            res = QString("%1: Qt.size(%2,%3)").arg(field).arg(size.width()).arg(size.height());
        } else {
            qDebug() << "Non-supported type" << v.typeName() << " Field :" << field;
            return QString("");
        }

        res = QString("").fill(' ', indent) + res;

        return res;
    };

    std::function<QString(QVariantMap, int)> _prettyText;

    _prettyText = [=, &_prettyText](QVariantMap snapshot, int indent) {
        if (snapshot.isEmpty()) {
            return QString("");
        }

        QStringList lines;

        if (snapshot.contains("$skip")) {
            QVariantList children = snapshot["$children"].toList();
            for (int i = 0 ; i < children.size() ;i++) {
                QVariantMap data = children[i].toMap();
                QString line = _prettyText(data, indent);
                if (!line.isEmpty())
                    lines << line;
            }
            return lines.join("\n");
        }

        int currentIndent = indent + 4;

        lines << QString().fill(' ',indent) + snapshot["$name"].toString() + " {";

        QStringList keys = snapshot.keys();

        if (keys.indexOf("id") >= 0) {
            lines << _prettyField("id", snapshot["id"], currentIndent).replace("\"","");
            keys.removeOne("id");
        }

        for (int i = 0 ; i < priorityFields.size() ; i++) {
            QString key = priorityFields[i];
            if (keys.indexOf(key) >= 0) {
                QString line = _prettyField(key, snapshot[key], currentIndent);
                if (!line.isEmpty()) {
                    lines << line;
                }
                keys.removeOne(key);
            }
        }

        for (int i = 0 ; i < keys.size();i++) {
            QString key = keys[i];
            if (key.indexOf("$") == 0) {
                continue;
            }
            QString line = _prettyField(key, snapshot[key], currentIndent);
            if (!line.isEmpty())
                lines << line;
        }

        QVariantList children = snapshot["$children"].toList();
        for (int i = 0 ; i < children.size() ;i++) {
            QVariantMap data = children[i].toMap();
            QString line = _prettyText(data, currentIndent);
            if (!line.isEmpty())
                lines << line;
        }

        lines << QString().fill(' ',indent) +  QString("}");

        return lines.join("\n");
    };

    return _prettyText(snapshot, 0);
}


void SnapshotTesting::setSnapshotsFile(const QString &file)
{
    m_snapshotFile = QtShell::realpath_strip(file);
}

QString SnapshotTesting::snapshotsFile()
{
    return m_snapshotFile;
}

QVariantMap SnapshotTesting::loadStoredSnapshots()
{
    if (!m_snapshots.isEmpty()) {
        return m_snapshots;
    }

    QVariantMap result;
    QString content = QtShell::cat(m_snapshotFile);

    if (content.isNull()) {
        return result;
    }

    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(content.toUtf8(),&error);

    if (error.error != QJsonParseError::NoError) {
        qWarning() << QString("SnapshotTesting::loadSnapshots: Failed to parse file: %1: %2").arg(m_snapshotFile).arg(error.errorString());
    }

    QVariantMap data = doc.object().toVariantMap();

    m_snapshots = data["content"].toMap();

    return m_snapshots;
}


void SnapshotTesting::saveSnapshots()
{
    if (m_snapshots.isEmpty()) {
        loadStoredSnapshots();
    }

    if (!m_snapshotsDirty) {
        return;
    }

    m_snapshotsDirty = false;

    QVariantMap data;

    data["content"] = m_snapshots;

    QJsonObject object = QJsonObject::fromVariantMap(data);

    QJsonDocument doc;
    doc.setObject(object);
    QByteArray bytes = doc.toJson(QJsonDocument::Indented);

    QFile file;
    file.setFileName(m_snapshotFile);
    if (!file.open(QIODevice::WriteOnly)) {
        qWarning() << QString("SnapshotTesting::saveSnapshots: Failed to save snapshos file: %1").arg(file.errorString());
        return;
    }

    file.write(bytes);
    file.close();
}

void SnapshotTesting::setSnapshot(const QString &name, const QString &content)
{
    m_snapshots[name] = content;
    m_snapshotsDirty = true;
}

void SnapshotTesting::setInteractiveEnabled(bool value)
{
    m_interactiveEnabled = value;
}

bool SnapshotTesting::interactiveEnabled()
{
    return m_interactiveEnabled;
}

void SnapshotTesting::setIgnoreAll(bool value)
{
    m_ignoreAll = value;
}

bool SnapshotTesting::ignoreAll()
{
    return m_ignoreAll;
}


QString SnapshotTesting::capture(QObject *object, SnapshotTesting::Options options)
{
    QVariantMap data = dehydrate(object, options);
    return prettyText(data);
}

bool SnapshotTesting::matchStoredSnapshot(const QString &name, const QString &snapshot)
{
    QVariantMap snapshots = SnapshotTesting::loadStoredSnapshots();

    /*
    if (!snapshots.contains(name)) {
        SnapshotTesting::setSnapshot(name, snapshot);
        SnapshotTesting::saveSnapshots();
        return true;
    }
    */

    QString originalVersion = snapshots[name].toString();

    if (originalVersion == snapshot) {
        return true;
    }

    QString diff = SnapshotTesting::diff(originalVersion, snapshot);

    qDebug().noquote() << "Snapshot::matchStoredSnapshot: The snapshot is different:";
    qDebug().noquote() << diff;

    if (m_acceptAll) {
        SnapshotTesting::setSnapshot(name, snapshot);
        SnapshotTesting::saveSnapshots();
        return true;
    }

    if (SnapshotTesting::interactiveEnabled() && !SnapshotTesting::ignoreAll()) {
        QQmlApplicationEngine engine;
        engine.addImportPath("qrc:///");
        engine.load(QUrl("qrc:/SnapshotTesting/Matcher.qml"));

        QObject* dialog = engine.rootObjects()[0];
        Q_ASSERT(dialog);

        dialog->setProperty("diff", diff);
        dialog->setProperty("previousSnapshot", originalVersion);
        dialog->setProperty("snapshot", snapshot);
        dialog->setProperty("title", name);

        QMetaObject::invokeMethod(dialog, "open");
        QCoreApplication::exec();

        int button = dialog->property("clickedButton").value<int>();
        switch (button) {
        // Use hex code to avoid the dependence to QtWidget
        case 0x00020000: // No to all
            SnapshotTesting::setIgnoreAll(true);
            break;
        case 0x00008000: // Yes to all
            m_acceptAll = true;
        case 0x00004000: // Yes
        case 0x02000000:
            SnapshotTesting::setSnapshot(name, snapshot);
            SnapshotTesting::saveSnapshots();
            return true;
            break;
        }
    }

    return false;
}

static void init() {
    m_snapshotFile = QtShell::realpath_strip(QtShell::pwd() + "snapshots.json");

    QString text = QtShell::cat(":/SnapshotTesting/config/snapshot-config.json");

    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(text.toUtf8(),&error);

    if (error.error != QJsonParseError::NoError) {
        qWarning() << "JSON::parse() error: "<< error.errorString();
    }

    QVariantMap map = doc.object().toVariantMap();
    knownComponentList = map.keys();
    for (int i = 0 ; i < knownComponentList.size() ; i++) {
        QString key = knownComponentList[i];
        QVariantMap record =  map[key].toMap();
        classNameToItemNameTable[key] = record["name"].toString();
        defaultValueMap[key] = record["defaultValues"].toMap();
        ignoreListMap[key] = record["ignoreList"].toStringList();
    }
}


QString SnapshotTesting::diff(QString original, QString current)
{
    auto toVector = [=](QString text) {
        vector<string> res;

        QStringList lines = text.split("\n");
        for (int i = 0 ; i < lines.size() ;i++) {
            res.push_back(lines[i].toStdString());
        }

        return res;
    };

    std::vector<string> text1, text2;

    text1 = toVector(original);
    text2 = toVector(current);
    dtl::Diff<std::string> diff(text1, text2);

    diff.onHuge();
    diff.compose();
    diff.composeUnifiedHunks();

    std::stringstream stream;

    diff.printUnifiedFormat(stream);

    return QString::fromStdString(stream.str());
}

Q_COREAPP_STARTUP_FUNCTION(init)
