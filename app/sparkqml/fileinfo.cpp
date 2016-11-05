#include "fileinfo.h"

FileInfo::FileInfo(QObject *parent) : QObject(parent)
{

}

QString FileInfo::completeBaseName(const QString &path)
{
    QFileInfo info(path);
    return info.completeBaseName();
}
