#include "model.h"

#include <QFont>
#include <QColor>
#include <QAction>
#include <QMessageBox>
#include <QRegExp>
#include <QDebug>


Model::Model(QObject *parent) :
    QAbstractTableModel(parent)
{
    items = new QList<Data*>;
    s_items = new QList<Data*>;
    all_items = new QList<Data*>;
}

int Model::Count()
{
    return items->count();
}

QList<Data *> *Model::Items()
{
    return this->items;
}

QList<Data *> *Model::All_Items()
{
    return this->all_items;
}

bool Model::checkReport(QString pattern, QString report)
{
    QRegExp rx(pattern);
    if(!checkRegExp(rx))return false;
    int pos = 0;
    while((pos = rx.indexIn(report, pos)) != -1){
        pos += rx.matchedLength();
        return true;
    }

    return false;
}

void Model::addItem(Data *item)
{
    if(item != NULL){
        items->append(item);
    }
    return;
}


void Model::setItems(QList<Data *> *items)
{
    this->items->clear();
    for(int i = 0; i < items->count(); i++){
        this->items->append(items->at(i));
    }
}


//Используется один раз при загрузке данных для копирования
void Model::setAllItems()
{
    for(int i = 0; i < items->count(); i++){
        this->all_items->append(items->at(i));
    }
}

void Model::clearItems()
{
    items->clear();
}


void Model::show_item(QModelIndex index)
{
    Data* item = getItem(index);

    if(!item){
        QMessageBox::information(NULL, "Сообщение", "Вы не выбрали ни одного элемента. "
                                       "Для выбора элемента кликните левой кнопокой мыши по нему.");
        return;
    }

    ShowItemWindow *show = new ShowItemWindow(item, pattern);

    connect(this, SIGNAL(shutdown_w()),
            show, SLOT(shutdown()));

    show->show();
}

void Model::selectAll()
{
    beginResetModel();
    endResetModel();
}


bool Model::checkRegExp(QRegExp rx)
{
    if(rx.isValid() && !rx.isEmpty() && !rx.exactMatch("")){
        return true;
    } else {
        QMessageBox::information(NULL,"Информсообщение",
                                 trUtf8("Некорректный шаблон регулярного выражения!"));
        return false;
    }
}

void Model::acceptPattern(QString pattern)
{
    this->pattern = pattern;
}


void Model::shutdown()
{
    emit shutdown_w();
}

QVariant Model::dataDisplay(const QModelIndex &index) const
{
    Data *item = items->at(index.row());
    switch (index.column()) {
    case 0: return item->Id();
    case 1: return item->Date().isValid() ? item->Date().toString("dd.MM.yyyy") : "";
    case 2: return item->Code_all();
    case 3: return item->Code_year();
    case 4: return item->Person();
    case 5: return item->Density_display();
    case 6: return item->Type() != "Sugar" ? item->Density() : item->Density() + 20;
    case 7: return item->Juice();
    case 8: return item->Type();
    case 9: return item->Start().isValid() ? item->Start().toString("hh.mm") : "";
    case 10: return item->End().isValid() ? item->End().toString("hh.mm") : "";
    case 11: return item->secondsToString();
    case 12: return item->dimensionsToString();
    case 13: return item->Description();
    default: return QVariant();
    }
}

QVariant Model::dataTextAlignment(const QModelIndex &) const
{

//    int Result = Qt::AlignBaseline;
    int Result = Qt::AlignVCenter;
//    Result = index.column() == 1 ? Qt::AlignLeft : Qt::AlignHCenter;
    return Result;
}


Data *Model::getItem(const QModelIndex &index) const
{
    int id = index.row();
    if(id < 0 || id >= items->size()) return 0;
    return items->at(id);
}


QVariant Model::dataToolTip(const QModelIndex &I) const
{
    Data *item = getItem(I);
    if(!item) return QVariant();

    switch (I.column()) {
    case 1:
    {
        if(!item->Date().isValid())return QVariant();
        return tr("%1").arg(item->Date().toString("dd.MM.yyyy"));
    }
    case 13: return item->Description();
    default: return QVariant();
    }
}

QVariant Model::dataBackground(const QModelIndex &index) const
{
    Data *item = items->at(index.row());
    QColor result;

    if(!item->isNew()) {
        if(item->Type() == "Premium"){
            result = QColor(240, 255, 255);
        } else if(item->Type() == "Yes" ){
            result = QColor(144, 238, 144);
        } else if(item->Type() == "Exclusive" ){
            result = QColor(32, 178, 170);
        } else if(item->Type() == "Sugar" ){
            result = QColor(255, 255, 0);
        } else {
            result = QColor("white");
        }
    } else {
        result = QColor("white");
    }

    result.setAlphaF(0.2);
    return result;

//QColor(189, 183, 107);
// QColor(255, 222, 173);
//QColor(240, 255, 255);

}


int Model::rowCount(const QModelIndex &parent) const
{
    //Если родитель существует, то кол-во строк 0
    if(!parent.isValid()){
        return items->count();
    } else {
        return 0;
    }
}

int Model::columnCount(const QModelIndex &parent) const
{
    if(!parent.isValid()){
        return 14;
    } else {
        return 0;
    }
}

QVariant Model::data(const QModelIndex &index, int role) const
{
    switch (role) {
    case Qt::DisplayRole:             return dataDisplay(index);
    case Qt::TextAlignmentRole: return dataTextAlignment(index);
    case Qt::ToolTipRole:             return dataToolTip(index);
    case Qt::BackgroundRole:    return dataBackground(index);
    default:  return QVariant();
    }
}

QVariant Model::headerData(int section, Qt::Orientation orientation, int role) const
{
    //Для горизонтальных заголовков оставляем данные по умолчанию
    if(orientation != Qt::Horizontal){
        return QAbstractTableModel::headerData(section,orientation,role);
    }

    switch (role) {
    case Qt::DisplayRole:
        switch (section) {
        case 0: return tr("Индекс");
        case 1: return tr("Дата");
        case 2: return tr("№");
        case 3: return tr("Номер2");
        case 4: return tr("Варщик");
        case 5: return tr("Плотность");
        case 6: return tr("Давл");
        case 7: return tr("Сок");
        case 8: return tr("Тип пасты");
        case 9: return tr("Начало");
        case 10: return tr("Конец");
        case 11: return tr("Время");
        case 12: return tr("Измерения");
        case 13: return tr("Комментарии");
        default: return QVariant();
        }
    case Qt::TextAlignmentRole:
        return QVariant(Qt::AlignBaseline | Qt::AlignHCenter);
    case Qt::ForegroundRole:
    {
        QFont F;
        F.setBold(true);
        return F;
    }
    default: return QVariant();
    }
}

Qt::ItemFlags Model::flags(const QModelIndex &) const
{
    return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}


//Глобальный поиск по всем элементам
void Model::search(QDate date, bool _date, QString person, bool _person,
                          QString type, bool _type, int density, bool _density, int flag,
                          QString pattern, bool _description){

    s_items->clear();
    Data *item = new Data();

    //Ищем по 1) Дата 2) Варщик 3) Тип пасты 4) Плотность 5) Комментарий
    if(_date && _person && _type && _density && _description){
        if(flag == 1){
            for(int i = 0; i < items->count(); i++){
                Data *item = items->at(i);
                if(item->Date().toString("dd.MM.yyyy").right(4) == date.toString("dd.MM.yyyy").right(4)){
                    if(item->Person() == person){
                        if(item->Type() == type){
                            if(item->Density() == density){
                                if(checkReport(pattern, item->Report())){
                                    s_items->append(item);
                            }
                        }
                        }
                    }
                }
            }
        } else if(flag == 2){
            for(int i = 0; i < items->count(); i++){
                item = items->at(i);
                if(item->Date().toString("dd.MM.yyyy").right(7) == date.toString("dd.MM.yyyy").right(7)){
                    if(item->Person() == person){
                        if(item->Type() == type){
                            if(item->Density() == density){
                                if(checkReport(pattern, item->Report())){
                                    s_items->append(item);
                                }
                            }
                        }
                    }
                }
            }
        } else if(flag == 3){
            for(int i = 0; i < items->count(); i++){
                item = items->at(i);
                if(item->Date().toString("dd.MM.yyyy") == date.toString("dd.MM.yyyy")){
                    if(item->Person() == person){
                        if(item->Type() == type){
                            if(item->Density() == density){
                                if(checkReport(pattern, item->Report())){
                                    s_items->append(item);
                                }
                            }
                        }
                    }
                }
            }
        }//Ищем по 1) Дата 2) Варщик 3) Тип пасты 4) Плотность
    } else if(_date && _person && _type && _density && !_description) {
        if(flag == 1){
            for(int i = 0; i < items->count(); i++){
                Data *item = items->at(i);
                if(item->Date().toString("dd.MM.yyyy").right(4) == date.toString("dd.MM.yyyy").right(4)){
                    if(item->Person() == person){
                        if(item->Type() == type){
                            if(item->Density() == density){
                                s_items->append(item);
                            }
                        }
                    }
                }
            }
        } else if(flag == 2){
            for(int i = 0; i < items->count(); i++){
                item = items->at(i);
                if(item->Date().toString("dd.MM.yyyy").right(7) == date.toString("dd.MM.yyyy").right(7)){
                    if(item->Person() == person){
                        if(item->Type() == type){
                            if(item->Density() == density){
                                s_items->append(item);
                            }
                        }
                    }
                }
            }
        } else if(flag == 3){
            for(int i = 0; i < items->count(); i++){
                item = items->at(i);
                if(item->Date().toString("dd.MM.yyyy") == date.toString("dd.MM.yyyy")){
                    if(item->Person() == person){
                        if(item->Type() == type){
                            if(item->Density() == density){
                                s_items->append(item);
                            }
                        }
                    }
                }
            }
        }//1) Дата 2) Варщик 3) Тип пасты
    } else if(_date && _person && _type && !_density && !_description){
        if(flag == 1){
            for(int i = 0; i < items->count(); i++){
                Data *item = items->at(i);
                if(item->Date().toString("dd.MM.yyyy").right(4) == date.toString("dd.MM.yyyy").right(4)){
                    if(item->Person() == person){
                        if(item->Type() == type){
                            s_items->append(item);
                        }
                    }
                }
            }
        } else if(flag == 2){
            for(int i = 0; i < items->count(); i++){
                item = items->at(i);
                if(item->Date().toString("dd.MM.yyyy").right(7) == date.toString("dd.MM.yyyy").right(7)){
                    if(item->Person() == person){
                        if(item->Type() == type){
                            s_items->append(item);
                        }
                    }
                }
            }
        } else if(flag == 3){
            for(int i = 0; i < items->count(); i++){
                item = items->at(i);
                if(item->Date().toString("dd.MM.yyyy") == date.toString("dd.MM.yyyy")){
                    if(item->Person() == person){
                        if(item->Type() == type){
                            s_items->append(item);
                        }
                    }
                }
            }
        }//1) Дата 2) Варщик
    } else if(_date && _person && !_type && !_density && !_description) {
        if(flag == 1){
            for(int i = 0; i < items->count(); i++){
                Data *item = items->at(i);
                if(item->Date().toString("dd.MM.yyyy").right(4) == date.toString("dd.MM.yyyy").right(4)){
                    if(item->Person() == person){
                        s_items->append(item);
                    }
                }
            }
        } else if(flag == 2){
            for(int i = 0; i < items->count(); i++){
                item = items->at(i);
                if(item->Date().toString("dd.MM.yyyy").right(7) == date.toString("dd.MM.yyyy").right(7)){
                    if(item->Person() == person){
                        s_items->append(item);
                    }
                }
            }
        } else if(flag == 3){
            for(int i = 0; i < items->count(); i++){
                item = items->at(i);
                if(item->Date().toString("dd.MM.yyyy") == date.toString("dd.MM.yyyy")){
                    if(item->Person() == person){
                        s_items->append(item);
                    }
                }
            }
        }//1) Дата
    } else if(_date && !_person && !_type && !_density && !_description){
        if(flag == 1){
            for(int i = 0; i < items->count(); i++){
                Data *item = items->at(i);
                if(item->Date().toString("dd.MM.yyyy").right(4) == date.toString("dd.MM.yyyy").right(4)){
                    s_items->append(item);
                }
            }
        } else if(flag == 2){
            for(int i = 0; i < items->count(); i++){
                item = items->at(i);
                if(item->Date().toString("dd.MM.yyyy").right(7) == date.toString("dd.MM.yyyy").right(7)){
                    s_items->append(item);
                }
            }
        } else if(flag == 3){
            for(int i = 0; i < items->count(); i++){
                item = items->at(i);
                if(item->Date().toString("dd.MM.yyyy") == date.toString("dd.MM.yyyy")){
                    s_items->append(item);
                }
            }
        }//1) Варщик 2) Тип пасты 3) Плотность 4) Комментарий
    } else if(!_date && _person && _type && _density && _description){
        for(int i = 0; i < items->count(); i++){
            Data *item = items->at(i);
            if(item->Person() == person){
                if(item->Type() == type){
                    if(item->Density() == density){
                        if(checkReport(pattern, item->Report())){
                            s_items->append(item);
                        }
                    }
                }
            }
        }//Ищем по 2) Варщик 3) Тип пасты 4) Плотность
    } else if(!_date && _person && _type && _density && !_description){
        for(int i = 0; i < items->count(); i++){
            Data *item = items->at(i);
            if(item->Person() == person){
                if(item->Type() == type){
                    if(item->Density() == density){
                        s_items->append(item);
                    }
                }
            }
        }//Ищем по 2) Варщик 3) Тип пасты
    } else if(!_date && _person && _type && !_density && !_description){
        for(int i = 0; i < items->count(); i++){
            Data *item = items->at(i);
            if(item->Person() == person){
                if(item->Type() == type){
                    s_items->append(item);
                }
            }
        }//Ищем по 2) Варщик
    } else if(!_date && _person && !_type && !_density && !_description){
        for(int i = 0; i < items->count(); i++){
            Data *item = items->at(i);
            if(item->Person() == person){
                s_items->append(item);
            }
        }//3) Тип пасты 4) Плотность 5) Комментарий
    } else if(!_date && !_person && _type && _density && _description){
        for(int i = 0; i < items->count(); i++){
            Data *item = items->at(i);
            if(item->Type() == type){
                if(item->Density() == density){
                    if(checkReport(pattern, item->Report())){
                        s_items->append(item);
                    }
                }
            }
        }//Ищем по 3) Тип пасты 4) Плотность
    } else if(!_date && !_person && _type && _density && !_description){
        for(int i = 0; i < items->count(); i++){
            Data *item = items->at(i);
            if(item->Type() == type){
                if(item->Density() == density){
                    s_items->append(item);
                }
            }
        }//Ищем по 3) Тип пасты
    } else if(!_date && !_person && _type && !_density && !_description){
        for(int i = 0; i < items->count(); i++){
            Data *item = items->at(i);
            if(item->Type() == type){
                s_items->append(item);
            }
        }//Ищем по 4) Плотность 5) Комментарий
    } else if(!_date && !_person && !_type && _density && _description){
        for(int i = 0; i < items->count(); i++){
            Data *item = items->at(i);
            if(item->Density() == density){
                if(checkReport(pattern, item->Report())){
                    s_items->append(item);
                }
            }
        }//Ищем по 4) Плотность
    } else if(!_date && !_person && !_type && _density && !_description){
        for(int i = 0; i < items->count(); i++){
            Data *item = items->at(i);
            if(item->Density() == density){
                s_items->append(item);
            }
        }//Ищем по 5) Комментарий
    } else if(!_date && !_person && !_type && !_density && _description){
        for(int i = 0; i < items->count(); i++){
            Data *item = items->at(i);
            if(checkReport(pattern, item->Report())){
                s_items->append(item);
            }
        }//Ищем по 1) Дата 2) Варщик 4) Плотность 5) Комментарий
    } else if(_date && _person && !_type && _density && _description){
        if(flag == 1){
            for(int i = 0; i < items->count(); i++){
                Data *item = items->at(i);
                if(item->Date().toString("dd.MM.yyyy").right(4) == date.toString("dd.MM.yyyy").right(4)){
                    if(item->Person() == person){
                        if(item->Density() == density){
                            if(checkReport(pattern, item->Report())){
                                s_items->append(item);
                            }
                        }
                    }
                }
            }
        } else if(flag == 2){
            for(int i = 0; i < items->count(); i++){
                item = items->at(i);
                if(item->Date().toString("dd.MM.yyyy").right(7) == date.toString("dd.MM.yyyy").right(7)){
                    if(item->Person() == person){
                        if(item->Density() == density){
                            if(checkReport(pattern, item->Report())){
                                s_items->append(item);
                            }
                        }
                    }
                }
            }
        } else if(flag == 3){
            for(int i = 0; i < items->count(); i++){
                item = items->at(i);
                if(item->Date().toString("dd.MM.yyyy") == date.toString("dd.MM.yyyy")){
                    if(item->Person() == person){
                        if(item->Density() == density){
                            if(checkReport(pattern, item->Report())){
                                s_items->append(item);
                            }
                        }
                    }
                }
            }
        }//1) Дата 2) Плотность 3) Комментарий
    } else if(_date && !_person && !_type && _density && _description){
        if(flag == 1){
            for(int i = 0; i < items->count(); i++){
                Data *item = items->at(i);
                if(item->Date().toString("dd.MM.yyyy").right(4) == date.toString("dd.MM.yyyy").right(4)){
                    if(item->Density() == density){
                        if(checkReport(pattern, item->Report())){
                            s_items->append(item);
                        }
                    }
                }
            }
        } else if(flag == 2){
            for(int i = 0; i < items->count(); i++){
                item = items->at(i);
                if(item->Date().toString("dd.MM.yyyy").right(7) == date.toString("dd.MM.yyyy").right(7)){
                    if(item->Density() == density){
                        if(checkReport(pattern, item->Report())){
                            s_items->append(item);
                        }
                    }
                }
            }
        } else if(flag == 3){
            for(int i = 0; i < items->count(); i++){
                item = items->at(i);
                if(item->Date().toString("dd.MM.yyyy") == date.toString("dd.MM.yyyy")){
                    if(item->Density() == density){
                        if(checkReport(pattern, item->Report())){
                            s_items->append(item);
                        }
                    }
                }
            }
        }//1) Дата 3) Комментарий
    } else if(_date && !_person && !_type && !_density && _description){
        if(flag == 1){
            for(int i = 0; i < items->count(); i++){
                Data *item = items->at(i);
                if(item->Date().toString("dd.MM.yyyy").right(4) == date.toString("dd.MM.yyyy").right(4)){
                    if(checkReport(pattern, item->Report())){
                        s_items->append(item);
                    }
                }
            }
        } else if(flag == 2){
            for(int i = 0; i < items->count(); i++){
                item = items->at(i);
                if(item->Date().toString("dd.MM.yyyy").right(7) == date.toString("dd.MM.yyyy").right(7)){
                    if(checkReport(pattern, item->Report())){
                        s_items->append(item);
                    }
                }
            }
        } else if(flag == 3){
            for(int i = 0; i < items->count(); i++){
                item = items->at(i);
                if(item->Date().toString("dd.MM.yyyy") == date.toString("dd.MM.yyyy")){
                    if(checkReport(pattern, item->Report())){
                        s_items->append(item);
                    }
                }
            }
        }//1) Дата 2) Тип пасты 3) Плотность 4) Комментарий
    } else if(_date && !_person && _type && _density && _description){
        if(flag == 1){
            for(int i = 0; i < items->count(); i++){
                Data *item = items->at(i);
                if(item->Date().toString("dd.MM.yyyy").right(4) == date.toString("dd.MM.yyyy").right(4)){
                    if(item->Type() == type){
                        if(item->Density() == density){
                            if(checkReport(pattern, item->Report())){
                                s_items->append(item);
                            }
                        }
                    }
                }
            }
        } else if(flag == 2){
            for(int i = 0; i < items->count(); i++){
                item = items->at(i);
                if(item->Date().toString("dd.MM.yyyy").right(7) == date.toString("dd.MM.yyyy").right(7)){
                    if(item->Type() == type){
                        if(item->Density() == density){
                            if(checkReport(pattern, item->Report())){
                                s_items->append(item);
                            }
                        }
                    }
                }
            }
        } else if(flag == 3){
            for(int i = 0; i < items->count(); i++){
                item = items->at(i);
                if(item->Date().toString("dd.MM.yyyy") == date.toString("dd.MM.yyyy")){
                    if(item->Type() == type){
                        if(item->Density() == density){
                            if(checkReport(pattern, item->Report())){
                                s_items->append(item);
                            }
                        }
                    }
                }
            }
        }//Ищем 1) Тип пасты 2) Комментарий
    } else if(!_date && !_person && _type && !_density && _description){
        for(int i = 0; i < items->count(); i++){
            Data *item = items->at(i);
            if(item->Type() == type){
                if(checkReport(pattern, item->Report())){
                    s_items->append(item);
                }
            }
        }//Ищем 1) Варщик 2) Плотность
    } else if(!_date && _person && !_type && _density && !_description){
        for(int i = 0; i < items->count(); i++){
            Data *item = items->at(i);
            if(item->Person() == person){
                if(item->Density() == density){
                    s_items->append(item);
                }
            }
        } // 1) Дата 2) Плотность
    } else if(_date && !_person && !_type && _density && !_description){
        if(flag == 1){
            for(int i = 0; i < items->count(); i++){
                Data *item = items->at(i);
                if(item->Date().toString("dd.MM.yyyy").right(4) == date.toString("dd.MM.yyyy").right(4)){
                    if(item->Density() == density){
                        s_items->append(item);
                    }
                }
            }
        } else if(flag == 2){
            for(int i = 0; i < items->count(); i++){
                item = items->at(i);
                if(item->Date().toString("dd.MM.yyyy").right(7) == date.toString("dd.MM.yyyy").right(7)){
                    if(item->Density() == density){
                        s_items->append(item);
                    }
                }
            }
        } else if(flag == 3){
            for(int i = 0; i < items->count(); i++){
                item = items->at(i);
                if(item->Date().toString("dd.MM.yyyy") == date.toString("dd.MM.yyyy")){
                    if(item->Density() == density){
                        s_items->append(item);
                    }
                }
            }
        }//Ищем 1) Дату 2) Тип пасты
    } else if(_date && !_person && _type && !_density && !_description){
        if(flag == 1){
            for(int i = 0; i < items->count(); i++){
                Data *item = items->at(i);
                if(item->Date().toString("dd.MM.yyyy").right(4) == date.toString("dd.MM.yyyy").right(4)){
                    if(item->Type() == type){
                        s_items->append(item);
                    }
                }
            }
        } else if(flag == 2){
            for(int i = 0; i < items->count(); i++){
                item = items->at(i);
                if(item->Date().toString("dd.MM.yyyy").right(7) == date.toString("dd.MM.yyyy").right(7)){
                    if(item->Type() == type){
                        s_items->append(item);
                    }
                }
            }
        } else if(flag == 3){
            for(int i = 0; i < items->count(); i++){
                item = items->at(i);
                if(item->Date().toString("dd.MM.yyyy") == date.toString("dd.MM.yyyy")){
                    if(item->Type() == type){
                        s_items->append(item);
                    }
                }
            }
        }//Ищем  1) Варщик 2) Плотность 3) Комментарий
    } else if(!_date && _person && !_type && _density && _description){
        for(int i = 0; i < items->count(); i++){
            Data *item = items->at(i);
            if(item->Person() == person){
                if(item->Density() == density){
                    if(checkReport(pattern, item->Report())){
                        s_items->append(item);
                    }
                }
            }
        }//Ищем по 1) Варщик 2) Тип пасты 3) Комментарий
    } else if(!_date && _person && _type && !_density && _description){
        for(int i = 0; i < items->count(); i++){
            Data *item = items->at(i);
            if(item->Person() == person){
                if(item->Type() == type){
                    if(checkReport(pattern, item->Report())){
                        s_items->append(item);
                    }
                }
            }
        }//1) Варщик 2) Комментарий
    } else if(!_date && _person && !_type && !_density && _description){
        for(int i = 0; i < items->count(); i++){
            Data *item = items->at(i);
            if(item->Person() == person){
                if(checkReport(pattern, item->Report())){
                    s_items->append(item);
                }
            }
        }//1) Дата 2) Варщик 3) Комментарий
    } else if(_date && _person && !_type && !_density && _description){
        if(flag == 1){
            for(int i = 0; i < items->count(); i++){
                Data *item = items->at(i);
                if(item->Date().toString("dd.MM.yyyy").right(4) == date.toString("dd.MM.yyyy").right(4)){
                    if(item->Person() == person){
                        if(checkReport(pattern, item->Report())){
                            s_items->append(item);
                        }
                    }
                }
            }
        } else if(flag == 2){
            for(int i = 0; i < items->count(); i++){
                item = items->at(i);
                if(item->Date().toString("dd.MM.yyyy").right(7) == date.toString("dd.MM.yyyy").right(7)){
                    if(item->Person() == person){
                        if(checkReport(pattern, item->Report())){
                            s_items->append(item);
                        }
                    }
                }
            }
        } else if(flag == 3){
            for(int i = 0; i < items->count(); i++){
                item = items->at(i);
                if(item->Date().toString("dd.MM.yyyy") == date.toString("dd.MM.yyyy")){
                    if(item->Person() == person){
                        if(checkReport(pattern, item->Report())){
                            s_items->append(item);
                        }
                    }
                }
            }
        }//1) Дата 2) Варщик 3) Плотность
    } else if(_date && _person && !_type && _density && !_description) {
        if(flag == 1){
            for(int i = 0; i < items->count(); i++){
                Data *item = items->at(i);
                if(item->Date().toString("dd.MM.yyyy").right(4) == date.toString("dd.MM.yyyy").right(4)){
                    if(item->Person() == person){
                            if(item->Density() == density){
                                s_items->append(item);
                            }
                    }
                }
            }
        } else if(flag == 2){
            for(int i = 0; i < items->count(); i++){
                item = items->at(i);
                if(item->Date().toString("dd.MM.yyyy").right(7) == date.toString("dd.MM.yyyy").right(7)){
                    if(item->Person() == person){
                            if(item->Density() == density){
                                s_items->append(item);
                            }
                    }
                }
            }
        } else if(flag == 3){
            for(int i = 0; i < items->count(); i++){
                item = items->at(i);
                if(item->Date().toString("dd.MM.yyyy") == date.toString("dd.MM.yyyy")){
                    if(item->Person() == person){
                            if(item->Density() == density){
                                s_items->append(item);
                            }
                        }
                }
            }
        }//1) Дата 2) Тип 3) Плотность
    } else if(_date && !_person && _type && _density && !_description){
        if(flag == 1){
            for(int i = 0; i < items->count(); i++){
                Data *item = items->at(i);
                if(item->Date().toString("dd.MM.yyyy").right(4) == date.toString("dd.MM.yyyy").right(4)){
                        if(item->Type() == type){
                            if(item->Density() == density){
                                s_items->append(item);
                            }
                    }
                }
            }
        } else if(flag == 2){
            for(int i = 0; i < items->count(); i++){
                item = items->at(i);
                if(item->Date().toString("dd.MM.yyyy").right(7) == date.toString("dd.MM.yyyy").right(7)){
                        if(item->Type() == type){
                            if(item->Density() == density){
                                s_items->append(item);
                            }
                        }
                }
            }
        } else if(flag == 3){
            for(int i = 0; i < items->count(); i++){
                item = items->at(i);
                if(item->Date().toString("dd.MM.yyyy") == date.toString("dd.MM.yyyy")){
                        if(item->Type() == type){
                            if(item->Density() == density){
                                s_items->append(item);
                            }
                        }
                }
            }
        }//Что-то ещё
    } else {
         qDebug() << "Нет параметра для поиска";
    }

    if(s_items->count() == 0){
        items->clear();
        return;
    }


    beginResetModel();
    setItems(s_items);
    endResetModel();
}

