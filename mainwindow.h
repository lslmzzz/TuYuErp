#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMdiSubWindow>
#include <QMdiArea>
#include "MysqlOperate/mysqloperate.h"
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
    void ShowLookFactoryContract(QString Contract_id);
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
    void ShowChengPinJieYuSlot();
};

#endif // MAINWINDOW_H
