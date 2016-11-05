#include <QGuiApplication>
#include <QtCore>
#include <QClipboard>
#include <QImage>
#include "clipboard.h"

Clipboard::Clipboard(QObject *parent) : QObject(parent)
{

}

void Clipboard::setImage(const QImage &image)
{
    QGuiApplication::clipboard()->setImage(image);
}
