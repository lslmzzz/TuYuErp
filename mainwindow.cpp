#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    mdiArea = new QMdiArea(this);
    this->setCentralWidget(mdiArea);
    //ע��˵�
    {//ϵͳ����
        QMenu* Q =  ui->menu;
        {
            QAction* action = Q->addAction(SS("��ӹ�����ͬ"));
            connect(action, SIGNAL(triggered()), this, SLOT(ShowAddFactoryContractSlot()));
        }
        {
            QAction* action = Q->addAction(SS("�鿴������ͬ����"));
            connect(action, SIGNAL(triggered()), this, SLOT(ShowFindFactoryContractSlot()));
        }
    }


}

MainWindow::~MainWindow()
{
    delete ui;
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
#include "factory_contract/addfactorycontract.h"

void MainWindow::ShowAddFactoryContractSlot(){
    QString Title = SS("��ӹ�����ͬ");
    QMdiSubWindow *wnd =GetSubWindow(Title);
    if( wnd == Q_NULLPTR){
        //mdiarea��Ӵ���
        AddFactoryContract *c1 = new AddFactoryContract;
        mdiArea->addSubWindow(c1);
        c1->setWindowState(Qt::WindowMaximized);
        c1->setWindowTitle(Title);
    }

}
#include "factory_contract/findfactorycontract.h"
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

#include "factory_contract/lookfactorycontract.h"
void MainWindow::ShowLookFactoryContract(QString Contract_id){
    QString Title = SS("������ͬ��ϸ");
    QMdiSubWindow *wnd =GetSubWindow(Title);
    if( wnd == Q_NULLPTR){
        //mdiarea��Ӵ���
        LookFactoryContract *c1 = new LookFactoryContract;
        c1->LoadData(Contract_id);
        mdiArea->addSubWindow(c1);
        c1->setWindowState(Qt::WindowMaximized);
        c1->setWindowTitle(Title);
    }else{
        ((LookFactoryContract *)wnd->widget())->LoadData(Contract_id);
    }
}
