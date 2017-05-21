#pragma once
#include <QObject>

class Tests : public QObject
{
    Q_OBJECT
public:
    explicit Tests(QObject *parent = 0);

private slots:
    void mockupLoadingTests();

    void MockupActor_createProject();

    void QmlEngine_scanImagePath();
    void QmlEngine_scanImagePath_withDefaultFile();

    void QmlFileListModel_test();

    void test_parseEnvFile();
};

