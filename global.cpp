#include "global.h"
CUser *CUser::local_instance = nullptr;


CUser *CUser::getInstance()
{
    if (local_instance == nullptr)
    {
        local_instance = new CUser();
    }
    return local_instance;
}


QString CUser::get_auth(QString MoKuaiName){
    auto mi = m_authmap.find(MoKuaiName);
    if(mi != m_authmap.end())
    {
        return mi.value();
    }
    return "";
}

bool CUser::LoadAuth() //����Ȩ��
{
   MysqlOperate *mysqlInstance = MysqlOperate::getInstance();
   QMap<QString,QString> conditions={{"role",m_role}};
   QVector<QMap<QString,QString>> data;
   MakeConditions makeconditions("user_auth");
   mysqlInstance->Find(makeconditions,data);
   if( data.size() == 0) return false;
   QJsonObject jo = QstringToJson(data[0].value("auth"));
   qDebug() << data[0].value("auth") << m_name;
   QJsonObject::Iterator it;
   m_authmap.clear();
   for(it=jo.begin();it!=jo.end();it++)
   {
       m_authmap.insert(it.key(),it.value().toString());
   }
   qDebug() << SS("�û���") << m_name;
   qDebug() << SS("�û���ɫ") << m_role;
   qDebug() << SS("�û�Ȩ��") << m_authmap;
   return true;
}

QJsonObject QstringToJson(QString jsonString)
{
    QJsonDocument jsonDocument = QJsonDocument::fromJson(jsonString.toStdString().data());
    if(jsonDocument.isNull())
    {
        QMessageBox::about(NULL, SS("������ʾ"), SS("jsonDocument is null"));
    }
    QJsonObject jsonObject = jsonDocument.object();
    return jsonObject;
}

QString JsonToQstring(QJsonObject jsonObject)
{
    return QString(QJsonDocument(jsonObject).toJson());
}



ClickQlabel::ClickQlabel(QWidget *parent, Qt::WindowFlags f):
    QLabel(parent,f)
{
}
ClickQlabel::ClickQlabel(const QString &text, QWidget *parent, Qt::WindowFlags f):
    QLabel(text,parent,f)
{
}
void ClickQlabel::mouseReleaseEvent(QMouseEvent * ev)
{
    Q_UNUSED(ev)
    emit clicked();
}

