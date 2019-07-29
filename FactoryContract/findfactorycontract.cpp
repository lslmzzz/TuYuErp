#include "findfactorycontract.h"
#include "ui_findfactorycontract.h"
#include "MysqlOperate/mysqloperate.h"
#include <QTableWidget>


FindFactoryContract::FindFactoryContract(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FindFactoryContract)
{
    ui->setupUi(this);
    page = 1;
    QTableWidget *m_tw = ui->tableWidget;
    m_tw->setColumnCount(5);
    m_tw->setRowCount(0);
    m_tw->setEditTriggers(QAbstractItemView::NoEditTriggers);
    QStringList headers;
    headers << SS("��ͬ���") << SS("ǩ���ص�") << SS("�׷���λ")<< SS("�ҷ���λ")
               << SS("����");
    m_tw->setHorizontalHeaderLabels(headers);

    ContractTableIndex.insert("contract_id",0);
    ContractTableIndex.insert("contract_address",1);
    ContractTableIndex.insert("jia_name",2);
    ContractTableIndex.insert("yi_name",3);
    m_tw->horizontalHeader()->setStyleSheet("QHeaderView::section{background:yellow;}");

}


FindFactoryContract::~FindFactoryContract()
{
    delete ui;
}



void FindFactoryContract::ShaiXuan(){
   QTableWidget *m_tw = ui->tableWidget;
   m_tw->clearContents();
   QString  contract_id = ui->lineEdit->text().trimmed();

   MysqlOperate *m_mysqloperate = MysqlOperate::getInstance();
   MakeConditions makeconditions("factory_contract");
   if(contract_id.length() >0 ) makeconditions.AddEqual("contract_id",contract_id);
   makeconditions.AddPage(1);
   QVector<QMap<QString,QString>> data;
   m_mysqloperate->Find(makeconditions,data);
   if( data.size() == 0 ) return ;

   m_tw->setRowCount(data.size());
   for(int i=0;i<data.size();i++){
       QMap<QString,QString> &rowdata = data[i];
       QMap<QString,int>::const_iterator itmap;
       for (itmap = ContractTableIndex.constBegin(); itmap != ContractTableIndex.constEnd(); ++itmap) {
           m_tw->setItem(i, itmap.value(),new QTableWidgetItem(rowdata.value(itmap.key())));
       }
       QPushButton *pBtn = new QPushButton(SS("�鿴����"));
       pBtn->setProperty("contract_id",rowdata.value("contract_id"));
       connect(pBtn, SIGNAL(clicked()), this, SLOT(ContractDetailSlot()));

       // ��QTableWidget����ӿؼ�
       m_tw->setCellWidget(i,4,pBtn);
   }

   int data_count = m_mysqloperate->Count(makeconditions);
   int total_page = data_count/PAGESIZE;
   if(data_count%PAGESIZE >0 ) total_page++;
   QString pagetext = QStringLiteral("�� %1 ҳ/ %2 ҳ").arg(1).arg(total_page);
   page = 1;
   ui->label_page->setText(pagetext);
}
#include <QMdiArea>
#include <QMdiSubWindow>
#include "mainwindow.h"
void FindFactoryContract::ContractDetailSlot(){
     QMdiArea *mdiArea  = ((QMdiSubWindow*)this->parent())->mdiArea();
     QPushButton *btn = (QPushButton *)sender();
     QString contract_id = btn->property("contract_id").toString();
     ((MainWindow*)mdiArea->parentWidget())->ShowLookFactoryContract(contract_id);
}


void FindFactoryContract::leftSlot(){
   page--;
   if( page<1) page=1;
   TiaoZhuang();
}
void FindFactoryContract::rightSlot(){
    page++;
    TiaoZhuang();
}

void FindFactoryContract::PageSlot(){

    int read_page = ui->lineEdit_page->text().toInt();
    page = read_page;
    TiaoZhuang();
}

void FindFactoryContract::TiaoZhuang(){
    QTableWidget *m_tw = ui->tableWidget;
    m_tw->clearContents();
    MysqlOperate *m_mysqloperate = MysqlOperate::getInstance();
    QString  contract_id = ui->lineEdit->text().trimmed();
    MakeConditions makeconditions("factory_contract");
    if(contract_id.length() >0 )makeconditions.AddEqual("contract_id",contract_id);

    int data_count = m_mysqloperate->Count(makeconditions);
    int total_page = data_count/PAGESIZE;
    if(data_count%PAGESIZE >0 ) total_page++;

    if( page < 1 || page > total_page) {
        page = 1;
    }

    QString pagetext = QStringLiteral("�� %1 ҳ/ %2 ҳ").arg(page).arg(total_page);
    ui->label_page->setText(pagetext);
    makeconditions.AddPage(page);
    QVector<QMap<QString,QString>> data;

    m_mysqloperate->Find(makeconditions,data);
    if( data.size() == 0 ) return ;

    m_tw->setRowCount(data.size());
    for(int i=0;i<data.size();i++){
        QMap<QString,QString> &rowdata = data[i];
        QMap<QString,int>::const_iterator itmap;
        for (itmap = ContractTableIndex.constBegin(); itmap != ContractTableIndex.constEnd(); ++itmap) {
            m_tw->setItem(i, itmap.value(),new QTableWidgetItem(rowdata.value(itmap.key())));
        }
        QPushButton *pBtn = new QPushButton(SS("�鿴����"));
        pBtn->setProperty("contract_id",rowdata.value("contract_id"));
        connect(pBtn, SIGNAL(clicked()), this, SLOT(ContractDetailSlot()));

        // ��QTableWidget����ӿؼ�
        m_tw->setCellWidget(i,4,pBtn);
    }
}

