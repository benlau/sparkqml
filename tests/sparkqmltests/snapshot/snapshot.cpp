#include <QFileInfo>
#include <QtShell>
#include <QTest>
#include <snapshot/snapshottesting.h>
#include <snapshot/snapshottools.h>
#include <QBuffer>
#include <QQmlApplicationEngine>
#include <QQuickItemGrabResult>
#include <QStack>
#include <functional>
#include <math.h>
#include <private/qqmldata_p.h>
#include <private/qqmlcontext_p.h>
#include "snapshot.h"


static QStringList knownComponentList;
static QMap<QString,QString> classNameToItemNameTable;

static QMap<QString, QVariantMap> defaultValueMap;
static QMap<QString, QStringList> ignoreListMap;

static QVariantMap dehydrate(const Snapshot& snapshot, QObject* source) {
    QString topLevelContextName;
    QQmlContext* topLevelContext = qmlContext(source);
    QStack<QString> contextStack;
    bool captureVisibleItemOnly = snapshot.captureVisibleItemOnly();
    bool expandAll = snapshot.expandAll();

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

    topLevelContextName = obtainContextName(source);
    contextStack.push(topLevelContextName);

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

        QString knownClassName = obtainKnownClassName(object);

        if (result.isNull()) {
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

    auto lastContext = [=]() {
        if (contextStack.size() == 0) {
            return QString("");
        }
        return contextStack.last();
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

        QString qml  = QString("import QtQuick 2.4\n %1 {}").arg(itemName);

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
        if (!id.isNull()) {
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

    auto _allowTravel = [=](QObject* object) {
        if (!captureVisibleItemOnly) {
            return true;
        }

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

    std::function<QVariantMap(QObject*)> travel;

    travel = [=, &travel, &contextStack](QObject* object) {
        if (!_allowTravel(object)) {
            return QVariantMap();
        }

        QVariantMap dest;
        QString contextName = obtainContextName(object);
        bool popOnQuit = false;

        if (lastContext() != contextName) {
            if (!expandAll && contextStack.size() >= 2) {
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
        QStringList lines;
        lines << QString().fill(' ',indent) + snapshot["$name"].toString() + " {";

        QStringList keys = snapshot.keys();

        if (keys.indexOf("id") >= 0) {
            lines << _prettyField("id", snapshot["id"], indent + 4).replace("\"","");
            keys.removeOne("id");
        }

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
    m_captureVisibleItemOnly = true;
    m_captureScreenshotEnabled = false;
    m_expandAll = false;
}

QString Snapshot::snapshotText() const
{
    return m_snapshotText;
}

QString Snapshot::previousSnapshotText() const
{
    return SnapshotTesting::loadSnapshots()[m_name].toString();
}

void Snapshot::capture(QObject *object)
{
    QVariantMap data = dehydrate(*this, object);
    m_snapshotText = prettyText(data);

    if (m_captureScreenshotEnabled) {
        QQuickItem* item = qobject_cast<QQuickItem*>(object);
        if (item) {
            QSharedPointer<QQuickItemGrabResult> result = item->grabToImage();
            QEventLoop loop;
            QObject::connect(result.data(), SIGNAL(ready()),&loop,SLOT(quit()));
            loop.exec();
            m_screenshot = result->image();
        }
    }
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
        SnapshotTesting::setSnapshot(m_name, m_snapshotText);
        SnapshotTesting::saveSnapshots();
        return true;
    }

    QString originalVersion = snapshots[m_name].toString();

    if (originalVersion == m_snapshotText) {
        return true;
    }

    QString diff = SnapshotTools::diff(originalVersion, m_snapshotText);

    qDebug().noquote() << "Snapshot::compare: The snapshot is different:";
    qDebug().noquote() << diff;

    if (SnapshotTesting::interactiveEnabled() && !SnapshotTesting::ignoreAll()) {
        QQmlApplicationEngine engine;
        engine.addImportPath("qrc:///");
        engine.load(QUrl("qrc:/SnapshotTesting/Matcher.qml"));

        QObject* dialog = engine.rootObjects()[0];
        Q_ASSERT(dialog);

        dialog->setProperty("diff", diff);
        dialog->setProperty("previousSnapshot", previousSnapshotText());
        dialog->setProperty("snapshot", m_snapshotText);
        dialog->setProperty("title", m_name);

        if (!m_screenshot.isNull()) {
            QByteArray ba;
            QBuffer buffer(&ba);
            buffer.open(QBuffer::WriteOnly);
            m_screenshot.save(&buffer, "PNG");
            buffer.close();
            QString base64 = QString("data:image/png;base64,") + ba.toBase64();
            QUrl source(base64);
            dialog->setProperty("screenshot", source);
        }

        QMetaObject::invokeMethod(dialog, "open");
        QCoreApplication::exec();

        int button = dialog->property("clickedButton").value<int>();
        switch (button) {
        // Use hex code to avoid the dependence to QtWidget
        case 0x00020000: // No to all
            SnapshotTesting::setIgnoreAll(true);
            break;
        case 0x02000000:
            SnapshotTesting::setSnapshot(m_name, m_snapshotText);
            SnapshotTesting::saveSnapshots();
            return true;
            break;
        }
    }

    return false;
}

bool Snapshot::captureVisibleItemOnly() const
{
    return m_captureVisibleItemOnly;
}

void Snapshot::setCaptureVisibleItemOnly(bool captureVisibleItemOnly)
{
    m_captureVisibleItemOnly = captureVisibleItemOnly;
}

bool Snapshot::captureScreenshotEnabled() const
{
    return m_captureScreenshotEnabled;
}

void Snapshot::setCaptureScreenshotEnabled(bool captureScreenshotEnabled)
{
    m_captureScreenshotEnabled = captureScreenshotEnabled;
}

QImage Snapshot::screenshot() const
{
    return m_screenshot;
}

bool Snapshot::expandAll() const
{
    return m_expandAll;
}

void Snapshot::setExpandAll(bool expendAll)
{
    m_expandAll = expendAll;
}

void Snapshot::setSnapshotText(const QString &snapshotText)
{
    m_snapshotText = snapshotText;
}

static void init() {
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

Q_COREAPP_STARTUP_FUNCTION(init)
