#include "jsonworker.h"

#include <QJsonDocument>
#include <QJsonArray>
#include <QVariantMap>

JsonWorker::JsonWorker(Model *model, QJsonObject object, QMutex *mutex, QObject *parent):
    QObject(parent), QRunnable ()
{
    this->model = model;
    this->object = object;
    this->mutex = mutex;
}

void JsonWorker::run()
{
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

    QMutexLocker locker(mutex);
    model->addItem(D);
}
