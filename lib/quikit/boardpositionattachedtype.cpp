#include "boardpositionattachedtype.h"

QUIKit::BoardPositionAttachedType::BoardPositionAttachedType(QObject *parent) : QObject(parent)
{
    m_row = 0;
    m_column = 0;
}

int QUIKit::BoardPositionAttachedType::row() const
{
    return m_row;
}

void QUIKit::BoardPositionAttachedType::setRow(int row)
{
    m_row = row;
    emit rowChanged();
}

int QUIKit::BoardPositionAttachedType::column() const
{
    return m_column;
}

void QUIKit::BoardPositionAttachedType::setColumn(int column)
{
    m_column = column;
    emit columnChanged();
}

QUIKit::BoardPositionAttachedType *QUIKit::BoardPositionAttachedType::qmlAttachedProperties(QObject *object)
{
    return new BoardPositionAttachedType(object);
}
