#ifndef MODEL_H
#define MODEL_H

#include <QObject>
#include <QAbstractTableModel>

#include "data.h"
#include "showitemwindow.h"

class Model : public QAbstractTableModel
{

    Q_OBJECT

    QList<Data*>*items;//Список указателей на элементы
    QList<Data*>*s_items;
    QList<Data*>*all_items;

    QString pattern;//для подсветки поиска по комментарию
    QString report;//Текст отчёта о варке
    QStringList lst;//список варщиков

public:
    Model(QObject *parent = 0);
    virtual ~Model(){}

    int Count();
    QList<Data*> *Items();
    QList<Data*> *All_Items();
    QStringList defineLstPerson();

    void show_item(QModelIndex index);
    void selectAll();
    bool checkReport(QString pattern, QString report);
    void search(QDate date, bool _date, QString person, bool _person,
                       QString type, bool _type, int density, bool _density, int flag,
                       QString pattern, bool _description);

    void addItem(Data *item);//В модель
    void setItems(QList<Data*>*items);
    void setAllItems();
    void clearItems();

    //Тестовые функции
    int getCount();
    Data *Item(int id);

private:
    bool checkRegExp(QRegExp rx);

private slots:
    void shutdown();
    void acceptPattern(QString pattern);//для подстветки
    //bool insert_to_db(Data *item);//для принятия из интернета данных

    // QSqlTableModel interface
public:
    virtual int rowCount(const QModelIndex &parent) const;
    virtual int columnCount(const QModelIndex &parent) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    virtual Qt::ItemFlags flags(const QModelIndex &) const;

    //Вспомогательные функции
protected:
    virtual QVariant    dataDisplay(const QModelIndex &index) const;
    virtual QVariant    dataTextAlignment(const QModelIndex &) const;
    virtual Data *getItem(const QModelIndex &index)const;
    virtual QVariant    dataToolTip(const QModelIndex &I) const;
    virtual QVariant    dataBackground(const QModelIndex &index) const;//Заливка ячейки

signals:
    void shutdown_w();
};

#endif // MODEL_H
