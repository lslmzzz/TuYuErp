#include "fuliaochuruku.h"
#include "ui_fuliaochuruku.h"

FuLiaoChuRuKu::FuLiaoChuRuKu(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FuLiaoChuRuKu)
{
    ui->setupUi(this);

    QIntValidator *IntValidator =new QIntValidator(0, 10000000);

    ui->LineEdit_shuliang->setValidator(IntValidator);
    ui->LineEdit_shuliang_2->setValidator(IntValidator);

    m_MysqlOperate = MysqlOperate::getInstance();

    QTableWidget *m_tb = ui->tableWidget_jieyu;
    m_tb->setColumnCount(5);
    m_tb->setRowCount(0);
    m_tb->setEditTriggers(QAbstractItemView::NoEditTriggers);
    QStringList head ;
    head << SS("��ⵥ��") << SS("��������") << SS("ʣ������")<<SS("��λ") <<SS("��ϸ");

    m_tb->setHorizontalHeaderLabels(head);
    m_tb->horizontalHeader()->setMinimumHeight(40);
    m_tb->horizontalHeader()->setStyleSheet("QHeaderView::section{background:yellow;font:13pt}");


}

FuLiaoChuRuKu::~FuLiaoChuRuKu()
{
    delete ui;
}


void FuLiaoChuRuKu::FuLiaoRukuSlot(){
    if( !RukuSlotValider()) return ;

    MysqlTableConfig* m_TableConfig = MysqlTableConfig::getInstance();
    QSet<QString> columns = m_TableConfig->get_table_columns("fuliao_ruku");
    QMap<QString,QString> RukuData;
    QSet<QString>::const_iterator itset;
    for (itset = columns.constBegin(); itset != columns.constEnd(); ++itset) {
        {
            QString objectName = "LineEdit_" + *itset;
            QLineEdit *object = this->findChild<QLineEdit *>(objectName);
            if( object != nullptr){
                RukuData.insert(*itset,object->text().trimmed());
                continue;
            }
        }
        {
            QString objectName = "dateEdit_" + *itset;
            QDateEdit *object = this->findChild<QDateEdit *>(objectName);
            if( object != nullptr){
                RukuData.insert(*itset,object->text().trimmed());
                continue;
            }
        }
    }
    RukuData.insert("jieyu_shuliang",RukuData.value("shuliang"));

    if( m_MysqlOperate->Insert("fuliao_ruku",RukuData)){
       QMessageBox::about(NULL, SS("��ʾ"), SS("�������ɹ�"));
    }

}
void FuLiaoChuRuKu::FuLiaoChukuSlot(){
    QMap<QString,QString> RuKudata;
    if( !ChukuSlotValider(RuKudata)) return ;
    MysqlTableConfig* m_TableConfig = MysqlTableConfig::getInstance();
    QSet<QString> columns = m_TableConfig->get_table_columns("fuliao_chuku");
    QMap<QString,QString> ChukuData;
    QSet<QString>::const_iterator itset;
    for (itset = columns.constBegin(); itset != columns.constEnd(); ++itset) {
        {
            QString objectName = "LineEdit_" + *itset + "_2"; //��2��ʾ����
            QLineEdit *object = this->findChild<QLineEdit *>(objectName);
            if( object != nullptr){
                ChukuData.insert(*itset,object->text().trimmed());
                continue;
            }
        }
        {
            QString objectName = "dateEdit_" + *itset + "_2"; //��2��ʾ����
            QDateEdit *object = this->findChild<QDateEdit *>(objectName);
            if( object != nullptr){
                ChukuData.insert(*itset,object->text().trimmed());
                continue;
            }
        }

    }

    QMap<QString,QString> RukuUpdateData={
        {"version",RuKudata.value("version")},
        {"jieyu_shuliang",RuKudata.value("jieyu_shuliang")},
    };

    MakeConditions c_ruku("fuliao_ruku");
    c_ruku.AddEqual("ruku_id",ui->LineEdit_ruku_id_2->text().trimmed()).AddEqual("version",RuKudata.value("version"));


    m_MysqlOperate->Transaction();
    bool s1 = m_MysqlOperate->Insert("fuliao_chuku",ChukuData);
    bool s2 = m_MysqlOperate->Update(c_ruku,RukuUpdateData);

    if(s1 && s2 ){
        QMessageBox::about(NULL, SS("��ʾ"), SS("���ϳ���ɹ�"));
        m_MysqlOperate->Commit();
    }else{
        QMessageBox::about(NULL, SS("��ʾ"), SS("���ϳ���ʧ��"));
        m_MysqlOperate->Rollback();
    }

}

bool FuLiaoChuRuKu::RukuSlotValider(){
    if( ui->LineEdit_ruku_id->text().trimmed().length() == 0){
        QMessageBox::about(NULL, SS("������ʾ"), SS("����д��ⵥ��"));
        return false;
    }
    if( ui->LineEdit_name->text().trimmed().length() == 0){
        QMessageBox::about(NULL, SS("������ʾ"), SS("����д��������"));
        return false;
    }
    if( ui->LineEdit_shuliang->text().trimmed().toInt() <= 0){
        QMessageBox::about(NULL, SS("������ʾ"), SS("��������"));
        return false;
    }
    return true;
}

bool FuLiaoChuRuKu::ChukuSlotValider(QMap<QString,QString> &RuKudata){
    QString RuKuId = ui->LineEdit_ruku_id_2->text().trimmed();
    if( RuKuId.length() == 0){
        QMessageBox::about(NULL, SS("������ʾ"), SS("����д��ⵥ��"));
        return false;
    }
    MakeConditions ruku_conditions("fuliao_ruku");
    MakeConditions chuku_conditions("fuliao_chuku");
    ruku_conditions.AddEqual("ruku_id",RuKuId);
    chuku_conditions.AddEqual("ruku_id",RuKuId);

    int shuliangRead =ui->LineEdit_shuliang_2->text().trimmed().toInt();
    if( shuliangRead == 0 ){
        QMessageBox::about(NULL, SS("������ʾ"), SS("���� ����Ϊ0"));
        return false;
    }

    m_MysqlOperate->Get(ruku_conditions,RuKudata);
    if(RuKudata.size() == 0 ){
        QMessageBox::about(NULL, SS("������ʾ"), SS("��ѯ����������ⵥ��"));
        return false;
    }

    int jieyu_shuliang = RuKudata.value("jieyu_shuliang").toInt();
    if( shuliangRead > jieyu_shuliang) {
        QMessageBox::about(NULL, SS("������ʾ"), SS("���������������,��ǰ��ⵥ���������:")+QString::number(jieyu_shuliang));
        return false;
    }
    RuKudata.insert("jieyu_shuliang",QString::number(jieyu_shuliang-shuliangRead));

    return true;
}

void FuLiaoChuRuKu::FuLiaoJieYuSlot(){
    QString Name = ui->LineEdit_name_3->text().trimmed();

    MakeConditions ruku_conditions("fuliao_ruku") ;

    if(Name.length() != 0 ) ruku_conditions.AddEqual("name",Name);

    if( ruku_conditions.Get().length() == 0)  {
        QMessageBox::about(NULL, SS("������ʾ"), SS("��ɸѡ����"));
        return;
    }

    MysqlOperate * m_MysqlOperate = MysqlOperate::getInstance();
    //��ȡ�ܽ���
    {
      qlonglong jieyu_shuliang = m_MysqlOperate->Sum("jieyu_shuliang",ruku_conditions);
      ui->LineEdit_shuliang_3->setText(QString::number(jieyu_shuliang));
    }

    TiaoZhuang(1);
}

void FuLiaoChuRuKu::TiaoZhuang(int page){

    MakeConditions ruku_cd("fuliao_ruku");
    QString Name = ui->LineEdit_name_3->text().trimmed();

    if(Name.length() != 0 ) ruku_cd.AddEqual("name",Name);
    if( ruku_cd.Get().length() == 0)  {
        QMessageBox::about(NULL, SS("������ʾ"), SS("��ɸѡ����"));
        return;
    }

    int data_count = m_MysqlOperate->Count(ruku_cd);
    int total_page = data_count/PAGESIZE;
    if(data_count%PAGESIZE >0 ) total_page++;
    if(page < 1 ) page = 1;
    else if(page > total_page) page = total_page;
    ruku_cd.AddPage(page);
    QString pagetext = QStringLiteral("�� %1 ҳ/ %2 ҳ").arg(page).arg(total_page);
    ui->label_page->setText(pagetext);

    //��ȡ����������data
    QVector<QMap<QString,QString>> data;
    m_MysqlOperate->Find(ruku_cd,data);

    QTableWidget *m_tb = ui->tableWidget_jieyu;
    m_tb->clearContents();
    m_tb->setRowCount(data.size());
    if(data.size() == 0){
        QMessageBox::about(NULL, SS("������ʾ"), SS("������"));
        return;
    }

    for(int i=0;i<data.size();i++){
        m_tb->setItem(i,0,new QTableWidgetItem(data[i].value("ruku_id")));
        m_tb->setItem(i,1,new QTableWidgetItem( data[i].value("name")));
        m_tb->setItem(i,2,new QTableWidgetItem(data[i].value("jieyu_shuliang")));
        m_tb->setItem(i,3,new QTableWidgetItem(data[i].value("danwei")));
        m_tb->setItem(i,4,new QTableWidgetItem(data[i].value("detail")));
    }
}

void FuLiaoChuRuKu::TiaoZhuangSlot(){
    int page = ui->lineEdit_page->text().trimmed().toInt();
    TiaoZhuang(page);
}
