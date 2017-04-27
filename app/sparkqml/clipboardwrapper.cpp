#include <QGuiApplication>
#include <QtCore>
#include <QClipboard>
#include <QImage>
#include "clipboardwrapper.h"

ClipboardWrapper::ClipboardWrapper(QObject *parent) : QObject(parent)
{

}

void ClipboardWrapper::setImage(const QImage &image)
{
    QGuiApplication::clipboard()->setImage(image);
}
