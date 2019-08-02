#include "findzhixiangcontract.h"
#include "ui_findzhixiangcontract.h"

FindZhiXiangContract::FindZhiXiangContract(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FindZhiXiangContract)
{
    ui->setupUi(this);

    m_MysqlOperate = MysqlOperate::getInstance();
    QTableWidget *m_tw = ui->tableWidget;
    m_tw->setColumnCount(7);
    m_tw->setRowCount(0);
    m_tw->setEditTriggers(QAbstractItemView::NoEditTriggers);
    QStringList headers;
    headers << SS("��ͬ���") << SS("�� ��") << SS("�� ��") << SS("ǩ������")
            << SS("�鿴") << SS("�༭") << SS("ɾ��");
    m_tw->setHorizontalHeaderLabels(headers);

    ContractTableIndex.insert("contract_id",0);
    ContractTableIndex.insert("xufang",1);
    ContractTableIndex.insert("gongfang",2);
    ContractTableIndex.insert("qianding_date",3);

    m_tw->horizontalHeader()->setStyleSheet("QHeaderView::section{background:yellow;}");


}

FindZhiXiangContract::~FindZhiXiangContract()
{
    delete ui;
}


void FindZhiXiangContract::ShaiXuan(){
   TiaoZhuang(1);
}
#include <QMdiArea>
#include <QMdiSubWindow>
#include "mainwindow.h"
void FindZhiXiangContract::ContractDetailSlot(){
    QMdiArea *mdiArea  = ((QMdiSubWindow*)this->parent())->mdiArea();
    QPushButton *btn = (QPushButton *)sender();
    QString contract_id = btn->property("contract_id").toString();
    ((MainWindow*)mdiArea->parentWidget())->ShowZhixiangContract(contract_id,e_mode::LOOK);
}

void FindZhiXiangContract::ContractEditSlot(){
    QMdiArea *mdiArea  = ((QMdiSubWindow*)this->parent())->mdiArea();
    QPushButton *btn = (QPushButton *)sender();
    QString contract_id = btn->property("contract_id").toString();
    ((MainWindow*)mdiArea->parentWidget())->ShowZhixiangContract(contract_id,e_mode::EDIT);
}


void FindZhiXiangContract::ContractDeleteSlot(){
    QTableWidget *m_tw = ui->tableWidget;
    QPushButton *btn = (QPushButton *)sender();
    QString contract_id = btn->property("contract_id").toString();
    int row = btn->property("row").toInt();

    MakeConditions mc_contract("zhixiang_contract");
    mc_contract.AddEqual("contract_id",contract_id);
    MakeConditions mc_product("zhixiang_product");
    mc_contract.AddEqual("contract_id",contract_id);
    switch( QMessageBox::warning(NULL, SS("����"),SS("ȷ��Ҫɾ���ú�ͬ��?�йظú�ͬ��������ݻᱻȫ��ɾ��"),
                                 QMessageBox::Yes | QMessageBox::Default,
                                 QMessageBox::No | QMessageBox::Escape ))
    {
    case QMessageBox::Yes:
        m_MysqlOperate->Transaction();
        bool ok2 = m_MysqlOperate->Delete(mc_product);
        bool ok1 = m_MysqlOperate->Delete(mc_contract);
        if( ok1 && ok2){
            m_MysqlOperate->Commit();
            m_tw->removeRow(btn->property("row").toInt());
            for(int i=0;i<m_tw->rowCount();i++){
                m_tw->cellWidget(i,6)->setProperty("row",i);
            }
            QMessageBox::about(NULL, SS("��ʾ"), SS("ɾ���ɹ�"));
        }else{
            m_MysqlOperate->Rollback();
            QMessageBox::about(NULL, SS("��ʾ"), SS("ɾ��ʧ��"));
        }
    }


}


void FindZhiXiangContract::PageSlot(){

    int read_page = ui->lineEdit_page->text().toInt();
    TiaoZhuang(read_page);
}

void FindZhiXiangContract::TiaoZhuang(int page){
    QTableWidget *m_tw = ui->tableWidget;
    m_tw->clearContents();
    QString  contract_id = ui->lineEdit_contract_id->text().trimmed();
    MakeConditions makeconditions("zhixiang_contract");
    if(contract_id.length() >0 )makeconditions.AddEqual("contract_id",contract_id);
    makeconditions.AddBetween("qianding_date",ui->dateEdit_begindate->text(),ui->dateEdit_enddate->text());
    QString  name = ui->lineEdit_name->text().trimmed();
    MakeConditions product_cd("zhixiang_product");
    if(name.length() >0 ) {
        product_cd.AddEqual("name",name);
        makeconditions.AddIn("contract_id","contract_id",product_cd);
    }

    int data_count = m_MysqlOperate->Count(makeconditions);
    int total_page = data_count/PAGESIZE;
    if(data_count%PAGESIZE >0 ) total_page++;
    if( page < 1) page = 1;
    else if(page > total_page ) page = total_page;
    QString pagetext = QStringLiteral("�� %1 ҳ/ %2 ҳ").arg(page).arg(total_page);
    ui->label_page->setText(pagetext);


    makeconditions.AddPage(page);
    QVector<QMap<QString,QString>> data;

    m_MysqlOperate->Find(makeconditions,data);
    m_tw->setRowCount(data.size());
    if( data.size() == 0 ) return ;

    for(int i=0;i<data.size();i++){
        QMap<QString,QString> &rowdata = data[i];
        QMap<QString,int>::const_iterator itmap;
        for (itmap = ContractTableIndex.constBegin(); itmap != ContractTableIndex.constEnd(); ++itmap) {
            m_tw->setItem(i, itmap.value(),new QTableWidgetItem(rowdata.value(itmap.key())));
        }
        {
           QPushButton *pBtn = new QPushButton(SS("�鿴����"));
           pBtn->setProperty("contract_id",rowdata.value("contract_id"));
           connect(pBtn, SIGNAL(clicked()), this, SLOT(ContractDetailSlot()));
           // ��QTableWidget����ӿؼ�
           m_tw->setCellWidget(i,4,pBtn);
        }
        {
           QPushButton *pBtn = new QPushButton(SS("�༭"));
           pBtn->setProperty("contract_id",rowdata.value("contract_id"));
           connect(pBtn, SIGNAL(clicked()), this, SLOT(ContractEditSlot()));
           // ��QTableWidget����ӿؼ�
           m_tw->setCellWidget(i,5,pBtn);
        }
        {
           QPushButton *pBtn = new QPushButton(SS("ɾ��"));
           pBtn->setProperty("row",i);
           pBtn->setProperty("contract_id",rowdata.value("contract_id"));
           connect(pBtn, SIGNAL(clicked()), this, SLOT(ContractDeleteSlot()));
           // ��QTableWidget����ӿؼ�
           m_tw->setCellWidget(i,6,pBtn);
        }
    }
}
