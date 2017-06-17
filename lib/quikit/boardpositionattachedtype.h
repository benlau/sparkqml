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
    Q_PROPERTY(int assignedRow READ assignedRow WRITE setAssignedRow NOTIFY assignedRowChanged)
    Q_PROPERTY(int assignedColumn READ assignedColumn WRITE setAssignedColumn NOTIFY assignedColumnChanged)

public:
    explicit BoardPositionAttachedType(QObject *parent = nullptr);

    int row() const;
    void setRow(int row);

    int column() const;
    void setColumn(int column);

    static BoardPositionAttachedType *qmlAttachedProperties(QObject *object);

    int assignedRow() const;
    void setAssignedRow(int assignedRow);

    int assignedColumn() const;
    void setAssignedColumn(int assignedColumn);

signals:
    void rowChanged();
    void columnChanged();
    void assignedRowChanged();
    void assignedColumnChanged();

public slots:

private:
    int m_row;
    int m_column;

    /// The row assigned by layout component
    int m_assignedRow;

    int m_assignedColumn;
};

}

QML_DECLARE_TYPEINFO(QUIKit::BoardPositionAttachedType, QML_HAS_ATTACHED_PROPERTIES)

#endif // BOARDPOSITIONATTACHEDTYPE_H
