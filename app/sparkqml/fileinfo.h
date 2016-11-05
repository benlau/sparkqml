#ifndef FILEINFO_H
#define FILEINFO_H

#include <QObject>
#include <QFileInfo>

class FileInfo : public QObject
{
    Q_OBJECT
public:
    explicit FileInfo(QObject *parent = 0);

signals:

public slots:
    QString completeBaseName(const QString& path);

};

#endif // FILEINFO_H
