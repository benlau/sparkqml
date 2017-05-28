#pragma once
#include <QObject>

class HotLoaderTests : public QObject
{
    Q_OBJECT
public:
    explicit HotLoaderTests(QObject *parent = 0);

private slots:

    void test_HotLoader_run();

    void test_HotLoader_mappedUrl();

};

