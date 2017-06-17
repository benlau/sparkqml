#include "boardpositionattachedtype.h"

QUIKit::BoardPositionAttachedType::BoardPositionAttachedType(QObject *parent) : QObject(parent)
{
    m_row = -1;
    m_column = -1;
    m_assignedRow = -1;
    m_assignedColumn = -1;
}

int QUIKit::BoardPositionAttachedType::row() const
{
    return m_row;
}

void QUIKit::BoardPositionAttachedType::setRow(int row)
{
    m_row = row;
    emit rowChanged();
    setAssignedRow(row);
}

int QUIKit::BoardPositionAttachedType::column() const
{
    return m_column;
}

void QUIKit::BoardPositionAttachedType::setColumn(int column)
{
    m_column = column;
    emit columnChanged();
    setAssignedColumn(column);
}

QUIKit::BoardPositionAttachedType *QUIKit::BoardPositionAttachedType::qmlAttachedProperties(QObject *object)
{
    return new BoardPositionAttachedType(object);
}

int QUIKit::BoardPositionAttachedType::assignedColumn() const
{
    return m_assignedColumn;
}

void QUIKit::BoardPositionAttachedType::setAssignedColumn(int assignedColumn)
{
    m_assignedColumn = assignedColumn;
    emit assignedColumnChanged();
}

int QUIKit::BoardPositionAttachedType::assignedRow() const
{
    return m_assignedRow;
}

void QUIKit::BoardPositionAttachedType::setAssignedRow(int assignedRow)
{
    m_assignedRow = assignedRow;
    emit assignedRowChanged();

}
