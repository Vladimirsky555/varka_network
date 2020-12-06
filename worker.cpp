#include "worker.h"

#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

#include <QThreadPool>
#include <QMutex>

#include <QDebug>

#include "data.h"


void Worker::setModel(Model *model)
{
    this->model = model;
}

void Worker::setArr(QByteArray arr)
{
    this->arr = arr;
}

void Worker::setNetReply(QNetworkReply *mNetReply)
{
    this->mNetReply = mNetReply;
}

void Worker::dataReadFinished()
{
    if( mNetReply->error())
    {
        qDebug() << "Error : " << mNetReply->errorString();
    }else
    {
       QJsonDocument doc = QJsonDocument::fromJson(arr);
       QJsonArray array = doc.array();

       QThreadPool* pool = QThreadPool::globalInstance();
       QMutex mutex(QMutex::Recursive);

       for ( int i = 0; i < array.size(); i++)
       {
           QJsonObject object = array.at(i).toObject();

           JsonWorker* jw = new JsonWorker(model, object, &mutex, nullptr);
           jw->setAutoDelete(true);
           pool->start(jw);

           emit sendCount(i+1);
       }

       pool->waitForDone();
    }

    emit workFinished();

}

void Worker::run()
{
    dataReadFinished();
}
