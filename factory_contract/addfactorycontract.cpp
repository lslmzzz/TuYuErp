#include "addfactorycontract.h"
#include "ui_addfactorycontract.h"
#include "MysqlOperate/mysqloperate.h"
AddFactoryContract::AddFactoryContract(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AddFactoryContract)
{
    ui->setupUi(this);

    //tablewidget������ֶ�
    QTableWidget *m_tw = ui->table_product;
    m_tw->setColumnCount(11);
    m_tw->setRowCount(0);
    m_tw->setEditTriggers(QAbstractItemView::NoEditTriggers);
    QStringList headers;
    headers << SS("��Ʒ����") << SS("��ɫ") << SS("���\n��ƽ�����أ�")<< SS("ë��")
            <<SS("��Ч�ŷ�/cm") <<  SS("��λ") <<SS("����") <<  SS("����/��˰/����\n��Ԫ/�ף�")
             <<SS("��Ԫ��") <<  SS("��ע") <<SS("����");
    m_tw->setHorizontalHeaderLabels(headers);

    ProductTableIndex.insert("name",0);
    ProductTableIndex.insert("color",1);
    ProductTableIndex.insert("spec",2);
    ProductTableIndex.insert("maogao",3);
    ProductTableIndex.insert("menfu",4);
    ProductTableIndex.insert("danwei",5);
    ProductTableIndex.insert("shuliang",6);
    ProductTableIndex.insert("danjia",7);
    ProductTableIndex.insert("jine",8);
    ProductTableIndex.insert("comment",9);

    m_tw->horizontalHeader()->setStyleSheet("QHeaderView::section{background:yellow;}");
}

AddFactoryContract::~AddFactoryContract()
{
    delete ui;
}

void AddFactoryContract::TianJiaProduct(){
    QTableWidget *m_tw = ui->table_product;
    int row = m_tw->rowCount();
    m_tw->setRowCount(row+1);
    m_tw->setItem(row,0,new QTableWidgetItem(ui->LineEdit_name->text().trimmed()));
    m_tw->setItem(row,1,new QTableWidgetItem(ui->LineEdit_color->text().trimmed()));
    m_tw->setItem(row,2,new QTableWidgetItem(ui->LineEdit_spec->text().trimmed()));
    m_tw->setItem(row,3,new QTableWidgetItem(ui->LineEdit_maogao->text().trimmed()));
    m_tw->setItem(row,4,new QTableWidgetItem(ui->LineEdit_menfu->text().trimmed()));
    m_tw->setItem(row,5,new QTableWidgetItem(ui->LineEdit_danwei->text().trimmed()));
    m_tw->setItem(row,6,new QTableWidgetItem(ui->LineEdit_shuliang->text().trimmed()));
    m_tw->setItem(row,7,new QTableWidgetItem(ui->LineEdit_danjia->text().trimmed()));
    m_tw->setItem(row,8,new QTableWidgetItem(ui->LineEdit_jine->text().trimmed()));
    m_tw->setItem(row,9,new QTableWidgetItem(ui->LineEdit_comment->text().trimmed()));
    QPushButton *pBtn = new QPushButton(SS("ɾ��"));
    pBtn->setProperty("Row",row);
    connect(pBtn, SIGNAL(clicked()), this, SLOT(DeleteProduct()));
    // ��QTableWidget����ӿؼ�
    m_tw->setCellWidget(row,10,pBtn);

}

void AddFactoryContract::DeleteProduct(){
   QTableWidget *m_tw = ui->table_product;
   QPushButton *btn = (QPushButton *)sender();
   m_tw->removeRow(btn->property("Row").toInt());
   for(int i=0;i<m_tw->rowCount();i++){
       m_tw->cellWidget(i,10)->setProperty("Row",i);
   }
}

#include "MysqlTableConfig/MysqlTableConfig.h"
void AddFactoryContract::AddContract(){
   MysqlTableConfig * m_TableConfig  = MysqlTableConfig::getInstance();
   MysqlOperate * m_MysqlOperate = MysqlOperate::getInstance();
   QSet<QString> columns = m_TableConfig->get_table_columns("factory_contract");
   QMap<QString,QString> Contractdata;
   QSet<QString>::const_iterator itset;
   for (itset = columns.constBegin(); itset != columns.constEnd(); ++itset) {
       { //����ǲ���lineedit
             QString objectName = "LineEdit_" + *itset;
             QLineEdit *lineEdit = this->findChild<QLineEdit *>(objectName);
             if( lineEdit != nullptr){
                 Contractdata.insert(*itset,lineEdit->text().trimmed());
                 continue;
             }
       }
       { //����ǲ���datetime
             QString objectName = "dateEdit_" + *itset;
             QDateEdit *object = this->findChild<QDateEdit *>(objectName);
             if( object != nullptr){
                 Contractdata.insert(*itset,object->text().trimmed());
                 continue;
             }
       }
   }
   if( !m_MysqlOperate->Insert("factory_contract",Contractdata) ){
       return ;
   }

   //���product
   QTableWidget *m_tw = ui->table_product;
   int row = m_tw->rowCount();

   for(int i=0;i<row;i++){
       QMap<QString,QString> mapdata;
       QMap<QString,int>::const_iterator itmap;
       mapdata.insert("contract_id",ui->LineEdit_contract_id->text().trimmed());
       for (itmap = ProductTableIndex.constBegin(); itmap != ProductTableIndex.constEnd(); ++itmap) {
           mapdata.insert(itmap.key(),m_tw->item(i,itmap.value())->text());
       }
       m_MysqlOperate->Insert("factory_product",mapdata);
   }

}
