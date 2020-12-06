#ifndef WORKER_H
#define WORKER_H

#include <QObject>
#include <QThread>
#include <QNetworkReply>

#include "model.h"
#include "jsonworker.h"

class Worker : public QThread
{  
    Q_OBJECT

    Model *model;
    QNetworkReply * mNetReply;
    QByteArray  arr;

public:
    Worker(QObject *parent = 0){}

    void setModel(Model *model);
    void setArr(QByteArray arr);
    void setNetReply(QNetworkReply *mNetReply);

private slots:
    void dataReadFinished();

signals:
    void workFinished();
    void sendCount(int);

    // QThread interface
protected:
    virtual void run();
};

#endif // WORKER_H
