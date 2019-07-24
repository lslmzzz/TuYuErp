#include "mainwindow.h"
#include <QApplication>
#include "MysqlTableConfig/MysqlTableConfig.h"
#include "MysqlOperate/mysqloperate.h"
#include "login.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MysqlTableConfig::getInstance();
    MainWindow w;
    Login lo;
    lo.exec();


    if( lo.get_login_success()){
         w.showMaximized();
    }else{
       return -1;
    }

    return a.exec();
}
