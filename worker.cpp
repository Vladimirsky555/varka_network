#include "worker.h"

#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QVariantMap>
#include <QDebug>

#include "data.h"

Worker::Worker(QObject *parent) :
    QThread(parent)
{

}

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

       for ( int i = 0; i < array.size(); i++)
       {
           QJsonObject object = array.at(i).toObject();
           QVariantMap map = object.toVariantMap();

           Data *D = new Data();
           QDate Date;

           D->setId(map["id"].toInt());
           D->setCode_all(map["code_all"].toInt());
           D->setCode_year(map["code_year"].toInt());                   
           D->setDate(Date.fromString(map["date"].toString(),"dd.MM.yyyy"));
           D->setPerson(map["person"].toString());
           D->setDensity(map["density"].toInt());
           D->setJuice(map["juice"].toInt());
           D->setType(map["type"].toString());
           D->setStart(map["start"].toTime());
           D->setEnd(map["end"].toTime());
           D->setDimensionFrom(map["dimensionFrom"].toInt());
           D->setDimensionTo(map["dimensionTo"].toInt());
           D->setTemperature(map["temperature"].toFloat());
           D->setDescription(map["description"].toString());

           emit sendCount(i+1);
           model->addItem(D);
       }
    }

    emit workFinished();
}

void Worker::run()
{
    dataReadFinished();
}
