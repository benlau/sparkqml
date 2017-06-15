#ifndef BOARDPOSITIONATTACHEDTYPE_H
#define BOARDPOSITIONATTACHEDTYPE_H

#include <QObject>
#include <QQmlEngine>

namespace QUIKit {

class BoardPositionAttachedType : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int row READ row WRITE setRow NOTIFY rowChanged)
    Q_PROPERTY(int column READ column WRITE setColumn NOTIFY columnChanged)

public:
    explicit BoardPositionAttachedType(QObject *parent = nullptr);

    int row() const;
    void setRow(int row);

    int column() const;
    void setColumn(int column);

    static BoardPositionAttachedType *qmlAttachedProperties(QObject *object);

signals:
    void rowChanged();
    void columnChanged();

public slots:

private:
    int m_row;
    int m_column;
};

}

QML_DECLARE_TYPEINFO(QUIKit::BoardPositionAttachedType, QML_HAS_ATTACHED_PROPERTIES)

#endif // BOARDPOSITIONATTACHEDTYPE_H
