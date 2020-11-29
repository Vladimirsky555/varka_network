#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>

#include "data.h"
#include "model.h"
#include "worker.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    Model *M;

    Worker *worker;
    QNetworkAccessManager * mNetManager;
    QNetworkReply * mNetReply;
    QByteArray  arr;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void createUI();
    void fillBoxes();
    bool check_index(int index);


private slots:
    void dataReadyRead();
    void dataReadFinished();
    void finishWorker();
    void acceptCount(int num);

    void on_btnAll_clicked();
    void on_btnSearch_clicked();
    void on_edtPattern_m_textChanged(const QString &str);
    void clearBoxes();
    void on_tableView_doubleClicked(const QModelIndex &index);


signals:
    void sendPattern(QString);//для подстветки
    void shutdown();

private:
    Ui::MainWindow *ui;

    // QWidget interface
protected:
    virtual void closeEvent(QCloseEvent *event);
};

#endif // MAINWINDOW_H
