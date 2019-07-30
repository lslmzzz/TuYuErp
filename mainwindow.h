#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMdiSubWindow>
#include <QMdiArea>
#include "MysqlOperate/mysqloperate.h"

#include "FactoryContract/addfactorycontract.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QMdiArea *mdiArea;
    QMdiSubWindow *GetSubWindow(QString title);
    void ShowFactoryContract(QString Contract_id,e_mode mode);
private:
    Ui::MainWindow *ui;
private slots:
    void ShowAddFactoryContractSlot();
    void ShowFindFactoryContractSlot();

    //����
    void ShowMianLiaoChuRuKuSlot();
    void ShowMianLiaoMingXiSlot();
    //����
    void ShowFuLiaoChuRuKuSlot();
    void ShowFuLiaoMingXiSlot();


    //��Ʒ
    void ShowChengPinChuRuKuSlot();
    void ShowChengPinMingXiSlot();
};

#endif // MAINWINDOW_H
