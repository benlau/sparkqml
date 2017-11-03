#ifndef PRIVATEAPITESTS_H
#define PRIVATEAPITESTS_H

#include <QObject>

class PrivateApiTests : public QObject
{
    Q_OBJECT
public:
    explicit PrivateApiTests(QObject *parent = nullptr);

signals:

private slots:
    void test_QQmlMetaType();
};

#endif // PRIVATEAPITESTS_H
