#ifndef JSONWORKER_H
#define JSONWORKER_H

#include <QObject>
#include <QRunnable>
#include <QJsonObject>
#include <QMutex>

#include "model.h"

class JsonWorker : public QObject, public QRunnable
{
    Q_OBJECT

    Model *model;
    QJsonObject object;
    QMutex* mutex;

public:
    JsonWorker(Model* model, QJsonObject object, QMutex *mutex, QObject *parent = nullptr);

    // QRunnable interface
public:
    virtual void run();
};

#endif // JSONWORKER_H
