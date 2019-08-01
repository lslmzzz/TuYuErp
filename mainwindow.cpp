#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "global.h"
#include <QLabel>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    mdiArea = new QMdiArea(this);
    this->setCentralWidget(mdiArea);

    {//���Statusbar

         welcome_label = new QLabel(SS("����:"));
         QToolBar *toolBar = ui->toolBar;
         toolBar->addWidget(welcome_label);
         toolBar->setStyleSheet("background-color:rgb(204,204,204);color:red");
    }
    //ע��˵�
    {//ϵͳ����
        QMenu* Q =  ui->menu;
        {
            QAction* action = Q->addAction(SS("������Ϻ�ͬ"));
            connect(action, SIGNAL(triggered()), this, SLOT(ShowAddFactoryContractSlot()));
        }
        {
            QAction* action = Q->addAction(SS("�鿴���Ϻ�ͬ����"));
            connect(action, SIGNAL(triggered()), this, SLOT(ShowFindFactoryContractSlot()));
        }
        {
            QAction* action = Q->addAction(SS("���ֽ���ͬ"));
            connect(action, SIGNAL(triggered()), this, SLOT(ShowAddZhiXiangContractSlot()));
        }
        {
            QAction* action = Q->addAction(SS("�鿴ֽ���ͬ����"));
            connect(action, SIGNAL(triggered()), this, SLOT(ShowFindZhiXiangContractSlot()));
        }
    }
    {//ϵͳ����
        QMenu* Q =  ui->menu_cangku;
        {
            QAction* action = Q->addAction(SS("���ϳ����"));
            connect(action, SIGNAL(triggered()), this, SLOT(ShowMianLiaoChuRuKuSlot()));
        }
        {
            QAction* action = Q->addAction(SS("������ϸ"));
            connect(action, SIGNAL(triggered()), this, SLOT(ShowMianLiaoMingXiSlot()));
        }

        {
            QAction* action = Q->addAction(SS("���ϳ����"));
            connect(action, SIGNAL(triggered()), this, SLOT(ShowFuLiaoChuRuKuSlot()));
        }
        {
            QAction* action = Q->addAction(SS("������ϸ"));
            connect(action, SIGNAL(triggered()), this, SLOT(ShowFuLiaoMingXiSlot()));
        }
        {
            QAction* action = Q->addAction(SS("��Ʒ�����"));
            connect(action, SIGNAL(triggered()), this, SLOT(ShowChengPinChuRuKuSlot()));
        }
        {
            QAction* action = Q->addAction(SS("��Ʒ��ϸ"));
            connect(action, SIGNAL(triggered()), this, SLOT(ShowChengPinMingXiSlot()));
        }
    }
    {//��������
        QMenu* Q =  ui->menu_dingdan;
        {
            QAction* action = Q->addAction(SS("��Ӷ�����ͬ"));
            connect(action, SIGNAL(triggered()), this, SLOT(ShowDingdanContractSlot()));
        }
        //{
        //    QAction* action = Q->addAction(SS("�鿴������ͬ����"));
        //    connect(action, SIGNAL(triggered()), this, SLOT(ShowFindFactoryContractSlot()));
        //}
    }





}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::SetWelcome(QString text){
    welcome_label->setText(text);
}

QMdiSubWindow * MainWindow::GetSubWindow(QString title){

    foreach (QMdiSubWindow *window, mdiArea->subWindowList())
    {
        if ( window->windowTitle() == title)
        {   //����Ѿ��򿪹������ý���
            window->showMaximized();
            return window;
        }
    }
    return Q_NULLPTR;
}
#include "FactoryContract/addfactorycontract.h"

void MainWindow::ShowAddFactoryContractSlot(){
    QString Title = SS("��ӹ�����ͬ");
    QMdiSubWindow *wnd =GetSubWindow(Title);
    if( wnd == Q_NULLPTR){
        //mdiarea��Ӵ���
        AddFactoryContract *c1 = new AddFactoryContract;
        c1->SetMode(e_mode::ADD);
        mdiArea->addSubWindow(c1);
        c1->setWindowState(Qt::WindowMaximized);
        c1->setWindowTitle(Title);
    }

}
#include "FactoryContract/findfactorycontract.h"
void MainWindow::ShowFindFactoryContractSlot(){
    QString Title = SS("�鿴������ͬ����");
    QMdiSubWindow *wnd =GetSubWindow(Title);
    if( wnd == Q_NULLPTR){
        //mdiarea��Ӵ���
        FindFactoryContract *c1 = new FindFactoryContract;
        mdiArea->addSubWindow(c1);
        c1->setWindowState(Qt::WindowMaximized);
        c1->setWindowTitle(Title);
    }

}
#include "ZhiXiangContract/zhixiangcontract.h"
void MainWindow::ShowAddZhiXiangContractSlot(){
    QString Title = SS("���ֽ���ͬ");
    QMdiSubWindow *wnd =GetSubWindow(Title);
    if( wnd == Q_NULLPTR){
        //mdiarea��Ӵ���
        ZhiXiangContract *c1 = new ZhiXiangContract;
        c1->SetMode(e_mode::ADD);
        mdiArea->addSubWindow(c1);
        c1->setWindowState(Qt::WindowMaximized);
        c1->setWindowTitle(Title);
    }
}

#include "ZhiXiangContract/findzhixiangcontract.h"
void MainWindow::ShowFindZhiXiangContractSlot(){
    QString Title = SS("�鿴ֽ���ͬ����");
    QMdiSubWindow *wnd =GetSubWindow(Title);
    if( wnd == Q_NULLPTR){
        //mdiarea��Ӵ���
        FindZhiXiangContract *c1 = new FindZhiXiangContract;
        mdiArea->addSubWindow(c1);
        c1->setWindowState(Qt::WindowMaximized);
        c1->setWindowTitle(Title);
    }
}


#include "FactoryContract/lookfactorycontract.h"
void MainWindow::ShowFactoryContract(QString Contract_id,e_mode mode){
    QString Title = SS("��ӹ�����ͬ");
    QMdiSubWindow *wnd =GetSubWindow(Title);
    if( wnd == Q_NULLPTR){
        //mdiarea��Ӵ���
        AddFactoryContract *c1 = new AddFactoryContract;
        c1->LoadData(Contract_id);
        c1->SetMode(mode);
        mdiArea->addSubWindow(c1);
        c1->setWindowState(Qt::WindowMaximized);
        c1->setWindowTitle(Title);
    }else{
        AddFactoryContract *c1 = ((AddFactoryContract *)wnd->widget());
        c1->LoadData(Contract_id);
        c1->SetMode(mode);
    }
}

void MainWindow::ShowZhixiangContract(QString Contract_id,e_mode mode){
    QString Title = SS("���ֽ���ͬ");
    QMdiSubWindow *wnd =GetSubWindow(Title);
    if( wnd == Q_NULLPTR){
        //mdiarea��Ӵ���
        ZhiXiangContract *c1 = new ZhiXiangContract;
        c1->LoadData(Contract_id);
        c1->SetMode(mode);
        mdiArea->addSubWindow(c1);
        c1->setWindowState(Qt::WindowMaximized);
        c1->setWindowTitle(Title);
    }else{
        ZhiXiangContract *c1 = ((ZhiXiangContract *)wnd->widget());
        c1->LoadData(Contract_id);
        c1->SetMode(mode);
    }

}

#include "Mianliao/miaoliaoruku.h"

void MainWindow::ShowMianLiaoChuRuKuSlot(){
    QString Title = SS("���ϳ����");
    QMdiSubWindow *wnd =GetSubWindow(Title);
    if( wnd == Q_NULLPTR){
        //mdiarea��Ӵ���
        MiaoliaoRuku *c1 = new MiaoliaoRuku;
        mdiArea->addSubWindow(c1);
        c1->setWindowState(Qt::WindowMaximized);
        c1->setWindowTitle(Title);
    }
}
#include "Mianliao/findmianliao.h"
void MainWindow::ShowMianLiaoMingXiSlot(){
    QString Title = SS("������ϸ");
    QMdiSubWindow *wnd =GetSubWindow(Title);
    if( wnd == Q_NULLPTR){
        //mdiarea��Ӵ���
        FindMianLiao *c1 = new FindMianLiao;
        mdiArea->addSubWindow(c1);
        c1->setWindowState(Qt::WindowMaximized);
        c1->setWindowTitle(Title);
    }

}


#include "FuLiao/fuliaochuruku.h"
void MainWindow::ShowFuLiaoChuRuKuSlot(){
    QString Title = SS("���ϳ����");
    QMdiSubWindow *wnd =GetSubWindow(Title);
    if( wnd == Q_NULLPTR){
        //mdiarea��Ӵ���
        FuLiaoChuRuKu *c1 = new FuLiaoChuRuKu;
        mdiArea->addSubWindow(c1);
        c1->setWindowState(Qt::WindowMaximized);
        c1->setWindowTitle(Title);
    }

}

#include "FuLiao/findfuliao.h"
void MainWindow::ShowFuLiaoMingXiSlot(){
    QString Title = SS("������ϸ");
    QMdiSubWindow *wnd =GetSubWindow(Title);
    if( wnd == Q_NULLPTR){
        //mdiarea��Ӵ���
        findfuliao *c1 = new findfuliao;
        mdiArea->addSubWindow(c1);
        c1->setWindowState(Qt::WindowMaximized);
        c1->setWindowTitle(Title);
    }


}


#include "ChengPin/chengpinchuruku.h"
void MainWindow::ShowChengPinChuRuKuSlot(){
    QString Title = SS("��Ʒ�����");
    QMdiSubWindow *wnd =GetSubWindow(Title);
    if( wnd == Q_NULLPTR){
        //mdiarea��Ӵ���
        ChengPinChuRuKu *c1 = new ChengPinChuRuKu;
        mdiArea->addSubWindow(c1);
        c1->setWindowState(Qt::WindowMaximized);
        c1->setWindowTitle(Title);
    }
}

#include "ChengPin/findchengpin.h"
void MainWindow::ShowChengPinMingXiSlot(){
    QString Title = SS("��Ʒ��ϸ");
    QMdiSubWindow *wnd =GetSubWindow(Title);
    if( wnd == Q_NULLPTR){
        //mdiarea��Ӵ���
        findchengpin *c1 = new findchengpin;
        mdiArea->addSubWindow(c1);
        c1->setWindowState(Qt::WindowMaximized);
        c1->setWindowTitle(Title);
    }

}
#include "DindanContract/dindancontract.h"


void MainWindow::ShowDingdanContractSlot(){
    QString Title = SS("������ͬ");
    QMdiSubWindow *wnd =GetSubWindow(Title);
    if( wnd == Q_NULLPTR){
        //mdiarea��Ӵ���
        DindanContract *c1 = new DindanContract;
        mdiArea->addSubWindow(c1);
        c1->setWindowState(Qt::WindowMaximized);
        c1->setWindowTitle(Title);
    }
}

