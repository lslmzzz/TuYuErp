#include "chengpinrukuedit.h"
#include "ui_chengpinrukuedit.h"

ChengPinRuKuEdit::ChengPinRuKuEdit(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChengPinRuKuEdit)
{
    ui->setupUi(this);

    m_MysqlOperate = MysqlOperate::getInstance();
    jieyu_shuliang = 0;
    shuliang = 0;

    QIntValidator *IntValidator =new QIntValidator(0, 10000000);
    ui->LineEdit_shuliang->setValidator(IntValidator);

}

ChengPinRuKuEdit::~ChengPinRuKuEdit()
{
    delete ui;
}



#include "MysqlTableConfig/MysqlTableConfig.h"
bool ChengPinRuKuEdit::RukuSlotValider(){
    if( ui->LineEdit_ruku_id->text().trimmed().length() == 0){
        QMessageBox::about(NULL, SS("������ʾ"), SS("����д��ⵥ��"));
        return false;
    }
    if( ui->LineEdit_name->text().trimmed().length() == 0){
        QMessageBox::about(NULL, SS("������ʾ"), SS("����д��Ʒ������"));
        return false;
    }
    int ShuliangRead =ui->LineEdit_shuliang->text().trimmed().toInt();

    if( ShuliangRead < (shuliang-jieyu_shuliang)) {
        QMessageBox::about(NULL, SS("������ʾ"), SS("����С�ڳ�����,��ǰ��ⵥ�������ѳ���:")+QString::number(shuliang-jieyu_shuliang));
        return false;
    }

    jieyu_shuliang = ShuliangRead -(shuliang-jieyu_shuliang);
    shuliang = ShuliangRead;
    return true;
}

void ChengPinRuKuEdit::ChengPinRukuEditSlot(){
    if( !RukuSlotValider()) return ;

    MysqlOperate * m_MysqlOperate = MysqlOperate::getInstance();
    MysqlTableConfig* m_TableConfig = MysqlTableConfig::getInstance();
    QSet<QString> columns = m_TableConfig->get_table_columns("chengpin_ruku");
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

    RukuData.insert("jieyu_shuliang",QString::number(jieyu_shuliang));

    MakeConditions cd("chengpin_ruku");
    cd.AddEqual("ruku_id",ruku_id);
    cd.AddEqual("version",QString::number(version));
    RukuData.insert("version",QString::number(version));

    if( m_MysqlOperate->Update(cd,RukuData)){
       QMessageBox::about(NULL, SS("��ʾ"), SS("���ϸ��³ɹ�"));
       this->close();
    }
}


bool ChengPinRuKuEdit::LoadData(QString Ruku_id){
    ruku_id =Ruku_id;
    MakeConditions cd("chengpin_ruku");
    cd.AddEqual("ruku_id",ruku_id);
    QMap<QString,QString> data;
    if( !m_MysqlOperate->Get(cd,data) ) return false;

    MysqlTableConfig* m_TableConfig = MysqlTableConfig::getInstance();
    QSet<QString> columns = m_TableConfig->get_table_columns("chengpin_ruku");

    QSet<QString>::const_iterator itset;
    for (itset = columns.constBegin(); itset != columns.constEnd(); ++itset) {
        {
            QString objectName = "LineEdit_" + *itset;
            QLineEdit *object = this->findChild<QLineEdit *>(objectName);
            if( object != nullptr){
                object->setText(data.value(*itset));
                continue;
            }
        }
        {
            QString objectName = "dateEdit_" + *itset;
            QDateEdit *object = this->findChild<QDateEdit *>(objectName);
            if( object != nullptr){
                object->setDate(QDate::fromString(data.value(*itset),"yyyy-MM-dd"));
                continue;
            }
        }
    }
    jieyu_shuliang = data.value("jieyu_shuliang").toInt();
    shuliang = data.value("shuliang").toInt();
    version = data.value("version").toInt();
    return true;
}
