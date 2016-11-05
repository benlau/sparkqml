#ifndef CLIPBOARD_H
#define CLIPBOARD_H

#include <QObject>

class Clipboard : public QObject
{
    Q_OBJECT
public:
    explicit Clipboard(QObject *parent = 0);

signals:

public slots:
    void setImage(const QImage& image);
};

#endif // CLIPBOARD_H
