#ifndef GLOBAL_H
#define GLOBAL_H
#include <QString>
#include <QMap>

class CUser //Ȩ����
{
  private:
    static CUser *local_instance;

  public:
    static CUser *getInstance();
    QString get_auth(QString MoKuaiName);
    QString get_name(){return m_name;};
    bool LoadAuth(); //����Ȩ��
    void set_role(QString role){m_role = role;}
  private:
    QString m_role; //�û���ɫ
    QString m_name; //�û�����
    QMap<QString, QString> m_authmap;
};
CUser *CUser::local_instance = nullptr;



#endif // GLOBAL_H
