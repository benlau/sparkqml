#pragma once
#include <QObject>

class ClipboardWrapper : public QObject
{
    Q_OBJECT
public:
    explicit ClipboardWrapper(QObject *parent = 0);

signals:

public slots:
    void setImage(const QImage& image);
};
