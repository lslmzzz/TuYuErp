#include "lookfactorycontract.h"
#include "ui_lookfactorycontract.h"
#include "MysqlOperate/mysqloperate.h"
#include "MysqlTableConfig/MysqlTableConfig.h"


LookFactoryContract::LookFactoryContract(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LookFactoryContract)
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
             <<SS("��Ԫ��") <<  SS("��ע");
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

LookFactoryContract::~LookFactoryContract()
{
    delete ui;
}


void LookFactoryContract::LoadData(QString Contract_id){
    MysqlOperate *m_MysqlOperate = MysqlOperate::getInstance();
    QMap<QString,QString> conditions = {{"contract_id",Contract_id}};
    QVector<QMap<QString,QString>> data;
    m_MysqlOperate->Find("factory_contract",&conditions,nullptr, data);
    if(data.size() == 0 ) return;
    MysqlTableConfig *m_TableConfig = MysqlTableConfig::getInstance();

    QSet<QString> columns = m_TableConfig->get_table_columns("factory_contract");
    QMap<QString,QString>::const_iterator itset;
    for (itset = data[0].constBegin(); itset != data[0].constEnd(); ++itset) {
        { //����ǲ���lineedit
              QString objectName = "LineEdit_" + itset.key();
              QLineEdit *lineEdit = this->findChild<QLineEdit *>(objectName);
              if( lineEdit != nullptr){
                  lineEdit->setText(itset.value());
                  continue;
              }
        }
        { //����ǲ���dateEdit_
              QString objectName = "dateEdit_" + itset.key();
              QDateEdit *object = this->findChild<QDateEdit *>(objectName);
              if( object != nullptr){
                  qDebug()<< itset.value();
                  object->setDate(QDate::fromString(itset.value(),"yyyy-MM-dd"));
                  continue;
              }
        }
    }
    ui->LineEdit_jia_name_2->setText( ui->LineEdit_jia_name->text());
    ui->LineEdit_yi_name_2->setText( ui->LineEdit_yi_name->text());

    // ���� widget
     m_MysqlOperate->Find("factory_product",&conditions,nullptr, data);
     QTableWidget *m_tw = ui->table_product;
     m_tw->clearContents();
     m_tw->setRowCount(data.size());
     for(int i = 0;i<data.size();i++){
         QMap<QString,int>::const_iterator itmap;
         QMap<QString,QString> &rowdata = data[i];
         for (itmap = ProductTableIndex.constBegin(); itmap != ProductTableIndex.constEnd(); ++itmap) {
             m_tw->setItem(i, itmap.value(),new QTableWidgetItem(rowdata.value(itmap.key())));
         }
     }
}
