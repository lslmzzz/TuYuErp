#ifndef GLOBAL_H
#define GLOBAL_H
#include <QString>
#include <QMap>
#include "MysqlOperate/mysqloperate.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
class CUser //Ȩ����
{
  private:
    static CUser *local_instance;

  public:
    static CUser *getInstance();
    QString get_auth(QString MoKuaiName);
    bool LoadAuth(); //����Ȩ��
    void set_role(QString role){m_role = role;}
    void set_name(QString name){m_name = name;}
    QString get_name(){return m_name;}
    QString get_role(){return m_role;}
  private:
    QString m_role; //�û���ɫ
    QString m_name; //�û�����
    QMap<QString, QString> m_authmap;
};

QJsonObject QstringToJson(QString jsonString);

QString JsonToQstring(QJsonObject jsonObject);

#endif // GLOBAL_H
