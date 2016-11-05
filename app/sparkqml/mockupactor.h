#ifndef MOCKUPACTOR_H
#define MOCKUPACTOR_H

#include <QObject>
#include <QFuture>

class MockupActor : public QObject
{
    Q_OBJECT
public:
    explicit MockupActor(QObject *parent = 0);
    virtual ~MockupActor();

    QString folder() const;
    void setFolder(const QString &folder);

    void init();

    QFuture<bool> createProject() const;

signals:

public slots:
private:
    class Context;
    Context* m_context;

    QString m_folder;
};

#endif // MOCKUPACTOR_H
