#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QVariantMap>
#include <QMessageBox>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    mNetManager(new QNetworkAccessManager(this)),
    mNetReply(NULL)
{
    ui->setupUi(this);
    setWindowTitle("Таблица учёта варок");

    M = new Model(this);
    worker = new Worker(this);
    worker->setModel(M);
    connect(worker, SIGNAL(workFinished()),this, SLOT(finishWorker()));
    connect(worker, SIGNAL(sendCount(int)),this, SLOT(acceptCount(int)));

    //Связываем кнопку с событием
    ui->btnClear->setDefaultAction(ui->actionClear);
    //Вызываем слот при нажатии на кнопку
    connect(ui->actionClear, SIGNAL(triggered()),
            this, SLOT(clearBoxes()));

    //Initialize our API data
    const QUrl API_ENDPOINT("http://myserver.ru");
    QNetworkRequest request;
    request.setUrl(API_ENDPOINT);
    mNetReply = mNetManager->get(request);
    connect(mNetReply,&QIODevice::readyRead,this,&MainWindow::dataReadyRead);
    connect(mNetReply,&QNetworkReply::finished,this,&MainWindow::dataReadFinished);

    ui->dateEdit_m->setDate(QDate::currentDate());
    QPixmap pix(":/images/pix.jpg");
    ui->label_pix->setPixmap(pix.scaledToWidth(250));
    fillBoxes();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createUI()
{
    ui->tableView->setModel(M);

    connect(this, SIGNAL(shutdown()),
            M, SLOT(shutdown()));

    connect(this, SIGNAL(sendPattern(QString)),
            M, SLOT(acceptPattern(QString)));

    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);

    ui->tableView->horizontalHeader()->setSectionResizeMode(14, QHeaderView::Stretch);
    ui->tableView->resizeColumnToContents(2);
    ui->tableView->resizeColumnToContents(5);
    ui->tableView->resizeColumnToContents(6);
    ui->tableView->resizeColumnToContents(8);
    ui->tableView->resizeColumnToContents(9);
    ui->tableView->resizeColumnToContents(10);
    ui->tableView->resizeColumnToContents(11);
    ui->tableView->resizeColumnToContents(12);
	
    ui->tableView->setColumnHidden(0, true);
    ui->tableView->setColumnHidden(3, true);
    ui->tableView->setColumnHidden(7, true);
    ui->tableView->setAlternatingRowColors(false);
}

void MainWindow::on_btnAll_clicked()
{
    M->clearItems();//очищаем модель перед поиском
    M->setItems(M->All_Items());//Заполняем модель всеми данными
    M->selectAll();//Обновляем представление
}

void MainWindow::on_btnSearch_clicked()
{
    QDate date;
    QString person;
    QString type;
    QString description;
    int density;
    int flag;

    bool _date;
    bool _person;
    bool _type;
    bool _density;
    bool _description;

    M->clearItems();//очищаем модель перед поиском
    M->setItems(M->All_Items());//Заполняем модель всеми данными
    M->selectAll();//Обновляем представление

    date = ui->dateEdit_m->date();

    if(ui->rYear_m->isChecked()){
        flag = 1;
    } else if(ui->rMonth_m->isChecked()){
        flag = 2;
    } else if(ui->rDay_m->isChecked()){
        flag = 3;
    }

    if(ui->cbxPerson_m->currentIndex() == 0){
        person = "---";
    } else if(ui->cbxPerson_m->currentIndex() == 1){
        person = "Артём";
    }else if(ui->cbxPerson_m->currentIndex() == 2){
        person = "Александр";
    }else if(ui->cbxPerson_m->currentIndex() == 3){
        person = "Владимир";
    }else if(ui->cbxPerson_m->currentIndex() == 4){
        person = "Яков";
    }else if(ui->cbxPerson_m->currentIndex() == 5){
        person = "Бато";
    } else if(ui->cbxPerson_m->currentIndex() == 5){
        person = "Аноним";
    }

    if(ui->cbxType_m->currentIndex() == 0){
        type = "---";
    } else if(ui->cbxType_m->currentIndex()  == 1){
        type = "Premium";
    }else if(ui->cbxType_m->currentIndex()  == 2){
        type = "Yes";
    }else if(ui->cbxType_m->currentIndex()  == 3){
        type = "Exclusive";
    }else if(ui->cbxType_m->currentIndex()  == 4){
        type = "Sugar";
    }

    if(ui->cbxDensity_m->currentIndex() == 0){
        density = 0;
    } else if(ui->cbxDensity_m->currentIndex() == 1){
        density = 100;
    }else if(ui->cbxDensity_m->currentIndex() == 2){
        density = 90;
    }else if(ui->cbxDensity_m->currentIndex() == 3){
        density = 80;
    }else if(ui->cbxDensity_m->currentIndex() == 4){
        density = 70;
    }else if(ui->cbxDensity_m->currentIndex() == 5){
        density = 60;
    }else if(ui->cbxDensity_m->currentIndex() == 6){
        density = 50;
    }

    description = ui->edtPattern_m->text();

    if(ui->cbDate->isChecked()){
        _date = true;
    } else {
        _date = false;
    }

    if(ui->edtPattern_m->text() == ""){
        _description = false;
    } else {
        _description = true;
    }

    _person = check_index(ui->cbxPerson_m->currentIndex());
    _type = check_index(ui->cbxType_m->currentIndex());
    _density = check_index(ui->cbxDensity_m->currentIndex());

    M->search(date, _date, person, _person, type, _type, density, _density, flag,
                     description, _description);
}

void MainWindow::fillBoxes()
{
    ui->cbxPerson_m->addItem("---");
    ui->cbxPerson_m->addItem("Артём");
    ui->cbxPerson_m->addItem("Александр");
    ui->cbxPerson_m->addItem("Владимир");
    ui->cbxPerson_m->addItem("Яков");
    ui->cbxPerson_m->addItem("Бато");
    ui->cbxPerson_m->addItem("Аноним");

    ui->cbxDensity_m->addItem("-------");
    ui->cbxDensity_m->addItem("0 (bandage)");
    ui->cbxDensity_m->addItem("1 (ultrasoft)");
    ui->cbxDensity_m->addItem("2 (soft)");
    ui->cbxDensity_m->addItem("3 (medium)");
    ui->cbxDensity_m->addItem("4 (hard)");
    ui->cbxDensity_m->addItem("5 (extrahard)");

    ui->cbxType_m->addItem("---");
    ui->cbxType_m->addItem("Premium");
    ui->cbxType_m->addItem("Yes");
    ui->cbxType_m->addItem("Exclusive");
    ui->cbxType_m->addItem("Sugar");
}

bool MainWindow::check_index(int index)
{
    if(index == 0){
        return false;
    }

    return true;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    emit shutdown();
    QWidget::closeEvent(event);
}


void MainWindow::on_edtPattern_m_textChanged(const QString &str)
{
    emit sendPattern(str);//отправляем поисковое слово для подстветки
}

void MainWindow::clearBoxes()
{
    ui->cbxDensity_m->setCurrentIndex(0);
    ui->cbxPerson_m->setCurrentIndex(0);
    ui->cbxType_m->setCurrentIndex(0);
    ui->edtPattern_m->setText("");
    ui->rYear_m->setChecked(true);
    ui->cbDate->setChecked(false);
}

void MainWindow::on_tableView_doubleClicked(const QModelIndex &index)
{
    M->show_item(index);
}

void MainWindow::finishWorker()
{
    M->setAllItems();
    createUI();
}

void MainWindow::acceptCount(int num)
{
    this->setWindowTitle("Общее количество варок: " + QString::number(num));
}

void MainWindow::dataReadyRead()
{
    arr.append(mNetReply->readAll());
}

void MainWindow::dataReadFinished()
{
    if(worker->isRunning()){
        worker->terminate();
    } else {
        worker->setNetReply(mNetReply);
        worker->setArr(arr);
        worker->start();
    }
}
