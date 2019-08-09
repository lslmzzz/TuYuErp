#include "kehucontract.h"
#include "ui_kehucontract.h"

KeHuContract::KeHuContract(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::KeHuContract)
{
    ui->setupUi(this);

    m_MysqlOperate = MysqlOperate::getInstance();
    //tablewidget������ֶ�
    QTableWidget *m_tw = ui->table_product;
    m_tw->setColumnCount(9);
    m_tw->setRowCount(0);
    QStringList headers;
    headers << SS("��Ʒ����") << SS("���ڻ���") << SS("���ߴ�")<< SS("ͼƬ")
            << SS("����") <<  SS("��λ") <<SS("����") << SS("���")
            << SS("ɾ��");
    m_tw->setHorizontalHeaderLabels(headers);

    ProductTableIndex.insert("name",0);
    ProductTableIndex.insert("chukouhuohao",1);
    ProductTableIndex.insert("spac",2);
    ProductTableIndex.insert("imageurl",3);
    ProductTableIndex.insert("shuliang",4);
    ProductTableIndex.insert("danwei",5);
    ProductTableIndex.insert("danjia",6);
    ProductTableIndex.insert("jine",7);

    m_tw->horizontalHeader()->setStyleSheet("QHeaderView::section{background:yellow;}");

}

KeHuContract::~KeHuContract()
{
    delete ui;
}


void KeHuContract::SetMode(e_mode mode){
    m_mode = mode;
    switch(m_mode){
     case e_mode::ADD:
        ui->pushButton->setText(SS("��Ӻ�ͬ"));
        ui->pushButton->setVisible(true);
        ui->LineEdit_contract_id->setReadOnly(false);
        break;
     case e_mode::EDIT:
        ui->pushButton->setText(SS("�޸ĺ�ͬ"));
        ui->pushButton->setVisible(true);
        ui->LineEdit_contract_id->setReadOnly(true);
        break;
     case e_mode::LOOK:
        ui->pushButton->setVisible(false);
        ui->LineEdit_contract_id->setReadOnly(true);
        break;
    }
}



void KeHuContract::DeleteProduct(){
    QTableWidget *m_tw = ui->table_product;
    QPushButton *btn = (QPushButton *)sender();
    m_tw->removeRow(btn->property("Row").toInt());
    for(int i=0;i<m_tw->rowCount();i++){
        m_tw->cellWidget(i,8)->setProperty("Row",i);
    }
}

void KeHuContract::ButtonSlot(){
    switch(m_mode){
    case e_mode::ADD:
        AddContract();
        break;
    case e_mode::EDIT:
        EditContract();
        break;
    default :
        break;
    }
}
#include "global.h"

#include "MysqlTableConfig/MysqlTableConfig.h"
void KeHuContract::AddContract(){
    if( ui->LineEdit_contract_id->text().trimmed().length() == 0) {
        QMessageBox::about(NULL, SS("������ʾ"), SS("��ͬ��Ų���Ϊ��"));
        return ;
    }
    MysqlTableConfig * m_TableConfig  = MysqlTableConfig::getInstance();
    QSet<QString> columns = m_TableConfig->get_table_columns("kehu_contract");
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

    //���product
    QTableWidget *m_tw = ui->table_product;
    int row = m_tw->rowCount();
    bool Success = true;
    m_MysqlOperate->Transaction();
    if( !m_MysqlOperate->Insert("kehu_contract",Contractdata)) Success = false;

    for(int i=0;i<row;i++){
        QMap<QString,QString> mapdata;
        QMap<QString,int>::const_iterator itmap;
        QString name = m_tw->item(i,0)->text().trimmed();
        if(name.length() == 0 ) continue;

        mapdata.insert("contract_id",ui->LineEdit_contract_id->text().trimmed());
        for (itmap = ProductTableIndex.constBegin(); itmap != ProductTableIndex.constEnd(); ++itmap) {
            if(itmap.key() == "imageurl"){
                ClickQlabel *label =  (ClickQlabel *)m_tw->cellWidget(i,3);
                QString UUid  =label->property("imageurl").toString();
                mapdata.insert(itmap.key(),UUid);
                continue;
            }else if(itmap.key() == "shuliang"){
                QLineEdit *LineEdit =  (QLineEdit *)m_tw->cellWidget(i,4);
                mapdata.insert(itmap.key(),LineEdit->text());
                continue;
            }else if(itmap.key() == "jine"){
                QLineEdit *LineEdit =  (QLineEdit *)m_tw->cellWidget(i,7);
                mapdata.insert(itmap.key(),LineEdit->text());
                continue;
            }
            mapdata.insert(itmap.key(),m_tw->item(i,itmap.value())->text().trimmed());
        }
        if( !m_MysqlOperate->Insert("kehu_product",mapdata) ) Success = false;
    }

    if( Success == false){
        m_MysqlOperate->Rollback();
        QMessageBox::about(NULL, SS("������ʾ"), SS("��ͬ���ʧ��"));
    }else{
        m_MysqlOperate->Commit();
        QMessageBox::about(NULL, SS("��ʾ"), SS("��ͬ��ӳɹ�"));
    }
}
#include <QMdiArea>
#include <QMdiSubWindow>
void KeHuContract::EditContract(){
    m_MysqlOperate->Transaction();
    MysqlTableConfig * m_TableConfig  = MysqlTableConfig::getInstance();
    QSet<QString> columns = m_TableConfig->get_table_columns("kehu_contract");
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


    //���product
    QTableWidget *m_tw = ui->table_product;
    int row = m_tw->rowCount();

    //ɾ��ȫ����Ʒ
    QString contract_id = ui->LineEdit_contract_id->text().trimmed();
    bool success = true;
    MakeConditions cds_product("kehu_product");
    cds_product.AddEqual("contract_id",contract_id);
    if( !m_MysqlOperate->Delete(cds_product) ) success = false;
    for(int i=0;i<row;i++){
        QMap<QString,QString> mapdata;
        QMap<QString,int>::const_iterator itmap;
        QString name = m_tw->item(i,0)->text().trimmed();
        if(name.length() == 0 ) continue;
        mapdata.insert("contract_id",ui->LineEdit_contract_id->text().trimmed());
        for (itmap = ProductTableIndex.constBegin(); itmap != ProductTableIndex.constEnd(); ++itmap) {
            if(itmap.key() == "imageurl"){
                ClickQlabel *label =  (ClickQlabel *)m_tw->cellWidget(i,3);
                QString UUid  =label->property("imageurl").toString();
                mapdata.insert(itmap.key(),UUid);
                continue;
            }else if(itmap.key() == "shuliang"){
                QLineEdit *LineEdit =  (QLineEdit *)m_tw->cellWidget(i,4);
                mapdata.insert(itmap.key(),LineEdit->text());
                continue;
            }else if(itmap.key() == "jine"){
                QLineEdit *LineEdit =  (QLineEdit *)m_tw->cellWidget(i,7);
                mapdata.insert(itmap.key(),LineEdit->text());
                continue;
            }
            mapdata.insert(itmap.key(),m_tw->item(i,itmap.value())->text().trimmed());
        }
        if( !m_MysqlOperate->Insert("kehu_product",mapdata) ) success = false;
    }

    MakeConditions cds_contract("kehu_contract");
    cds_contract.AddEqual("contract_id",contract_id).AddEqual("version",version);
    Contractdata.insert("version",version);
    if( ! m_MysqlOperate->Update(cds_contract,Contractdata) ) success = false;

    if( success == false){
        m_MysqlOperate->Rollback();
        QMessageBox::about(NULL, SS("������ʾ"), SS("��ͬ����ʧ��"));
    }else{
        m_MysqlOperate->Commit();
        QMessageBox::about(NULL, SS("��ʾ"), SS("��ͬ���³ɹ�"));
        QMdiArea *mdiArea  = ((QMdiSubWindow*)this->parent())->mdiArea();
        mdiArea->closeActiveSubWindow();
    }

}

#include "global.h"
void KeHuContract::AddLine(){
    QTableWidget *m_tw = ui->table_product;
    int rowCount = m_tw->rowCount();
    m_tw->setRowCount(rowCount+1);

    QPushButton *pBtn = new QPushButton(SS("ɾ��"));
    pBtn->setProperty("Row",rowCount);
    connect(pBtn, SIGNAL(clicked()), this, SLOT(DeleteProduct()));
    // ��QTableWidget����ӿؼ�
    m_tw->setCellWidget(rowCount,8,pBtn);
    for(int i=0;i<8;i++){
        if( i == 3){//ͼƬ
            ClickQlabel *  label = new ClickQlabel(SS("�������ͼƬ"));
            connect(label, SIGNAL(clicked()), this, SLOT(LoadImage()));
            m_tw->setCellWidget(rowCount,i,label);
            m_tw->setColumnWidth(i,150);
            continue;
        }
        if( i == 4){//����
            QLineEdit *LineEdit = new QLineEdit("");
            m_tw->setCellWidget(rowCount,i,LineEdit);
            QIntValidator *Validator =new QIntValidator(0, 10000000);
            LineEdit->setValidator(Validator);
            continue;
        }
        if( i == 7){//����
            QLineEdit *LineEdit = new QLineEdit("");
            m_tw->setCellWidget(rowCount,i,LineEdit);
            QDoubleValidator *Validator =new QDoubleValidator(0, 10000000,3);
            LineEdit->setValidator(Validator);
            continue;
        }
       m_tw->setItem(rowCount,i,new QTableWidgetItem(""));
    }
}

#include <QFileDialog>
#include "picture_client/qtclient.h"

void KeHuContract::LoadImage(){

     if( m_mode == e_mode::LOOK) return;
     ClickQlabel *label = (ClickQlabel *)sender();

     QStringList    fileNameList;
     QString fileName0;
     QFileDialog* fd = new QFileDialog(this);//�����Ի���
     fd->resize(240,320);    //������ʾ�Ĵ�С
     fd->setNameFilter( ("png(*.png);;jpg(*.jpg);;jpeg(*.jpeg)")); //�����ļ�������
     fd->setViewMode(QFileDialog::List);  //�������ģʽ,���б�list�� ģʽ�� ��ϸ��Ϣ��detail�����ַ�ʽ
     if ( fd->exec() == QDialog::Accepted )   //����ɹ���ִ��
     {
         fileNameList = fd->selectedFiles();      //�����ļ��б������
         QString fileName0 = fileNameList[0];            //ȡ��һ���ļ���
         //�ϴ�ͼƬ
         QtClient client;
         client.UploadData(fileName0);
         if(client.Success){
             label->setProperty("imageurl",client.uuid);
             QPixmap fitpixmap = QPixmap(fileName0).scaled(150, 120, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
             label->setPixmap(fitpixmap);
         }else{
             QMessageBox::about(NULL, SS("������ʾ"), SS("ͼƬ�ϴ�ʧ��"));

         }
     }
     else fd->close();
}
#include "picture_client/msg_def.h"
bool KeHuContract::LoadData(QString ContractId){
    MakeConditions cd_contract("kehu_contract");
    cd_contract.AddEqual("contract_id",ContractId);
    MakeConditions cd_product("kehu_product");
    cd_product.AddEqual("contract_id",ContractId);


    QMap<QString,QString> data_contract;
    if( !m_MysqlOperate->Get(cd_contract,data_contract) ) return false;

    MysqlTableConfig* m_TableConfig = MysqlTableConfig::getInstance();
    QSet<QString> columns = m_TableConfig->get_table_columns("kehu_contract");

    QSet<QString>::const_iterator itset;
    for (itset = columns.constBegin(); itset != columns.constEnd(); ++itset) {
        {
            QString objectName = "LineEdit_" + *itset;
            QLineEdit *object = this->findChild<QLineEdit *>(objectName);
            if( object != nullptr){
                object->setText(data_contract.value(*itset));
                continue;
            }
        }
        {
            QString objectName = "dateEdit_" + *itset;
            QDateEdit *object = this->findChild<QDateEdit *>(objectName);
            if( object != nullptr){
                object->setDate(QDate::fromString(data_contract.value(*itset),"yyyy-MM-dd"));
                continue;
            }
        }
    }

    version = data_contract.value("version");


    QVector<QMap<QString,QString>> data_product;
    if( !m_MysqlOperate->Find(cd_product,data_product) ) return false;

    QTableWidget *m_tw = ui->table_product;
    m_tw->clearContents();
    m_tw->setRowCount(data_product.size());
    for(int i = 0;i<data_product.size();i++){
        QMap<QString,int>::const_iterator itmap;
        QMap<QString,QString> &rowdata = data_product[i];
        for (itmap = ProductTableIndex.constBegin(); itmap != ProductTableIndex.constEnd(); ++itmap) {

            if(itmap.key() == "imageurl" ){
                ClickQlabel *label = new ClickQlabel("������ͼƬ");
                connect(label, SIGNAL(clicked()), this, SLOT(LoadImage()));
                m_tw->setCellWidget(i,3,label);
                m_tw->setColumnWidth(3,150);
                QtClient Client;
                QString uuid=rowdata.value(itmap.key());
                Client.DownData(uuid);
                if(Client.Success ){
                      label->setProperty("imageurl",uuid);
                      QPixmap fitpixmap = QPixmap(save_dir + uuid).scaled(150, 120, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
                      label->setPixmap(fitpixmap);
                }else{
                    return false;
                }
                continue;
            }
            else if(itmap.key() == "shuliang" ){
                 QLineEdit *LineEdit = new QLineEdit(rowdata.value(itmap.key()));
                 m_tw->setCellWidget(i,4,LineEdit);
                 QIntValidator *Validator =new QIntValidator(0, 10000000);
                 LineEdit->setValidator(Validator);
                 continue;
            }else if(itmap.key() == "jine"){
                QLineEdit *LineEdit = new QLineEdit(rowdata.value(itmap.key()));
                m_tw->setCellWidget(i,7,LineEdit);
                QDoubleValidator *Validator =new QDoubleValidator(0, 10000000,3);
                LineEdit->setValidator(Validator);
                continue;
            }

            m_tw->setItem(i, itmap.value(),new QTableWidgetItem(rowdata.value(itmap.key())));
        }
        QPushButton *pBtn = new QPushButton(SS("ɾ��"));
        pBtn->setProperty("Row",i);
        connect(pBtn, SIGNAL(clicked()), this, SLOT(DeleteProduct()));
        // ��QTableWidget����ӿؼ�
        m_tw->setCellWidget(i,8,pBtn);

    }
    return true;
}



void KeHuContract::HeJiShuLiang(){
    QTableWidget *m_tw = ui->table_product;
    int row = m_tw->rowCount();

    for(int i=0;i<row;i++){
        QMap<QString,QString> mapdata;
        QMap<QString,int>::const_iterator itmap;
        QString name = m_tw->item(i,0)->text().trimmed();
        if(name.length() == 0 ) continue;

    }
}
void KeHuContract::HeJiJine(){
    QTableWidget *m_tw = ui->table_product;
    int row = m_tw->rowCount();
    int shuliang = 0;
    double jine = 0.0;
    for(int i=0;i<row;i++){
        QString name = m_tw->item(i,0)->text().trimmed();
        if(name.length() == 0 ) continue;
        {
           QLineEdit *LineEdit = (QLineEdit *)m_tw->cellWidget(i,4);
           shuliang += LineEdit->text().toInt();
        }
        {
           QLineEdit *LineEdit = (QLineEdit *)m_tw->cellWidget(i,7);
           jine += LineEdit->text().toDouble();
        }
    }
    ui->LineEdit_heji_shuliang->setText(QString::number(shuliang));
    ui->LineEdit_heji_jine->setText(QString::number(jine));

}

