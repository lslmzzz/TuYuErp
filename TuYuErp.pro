#-------------------------------------------------
#
# Project created by QtCreator 2019-07-22T22:51:03
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TuYuErp
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        mainwindow.cpp \
    global.cpp \
    MysqlOperate/mysqloperate.cpp \
    MysqlOperate/mysqloperate.cpp \
    MysqlTableConfig/MysqlTableConfig.cpp \
    login.cpp \
    login.cpp \
    factory_contract/addfactorycontract.cpp \
    factory_contract/lookfactorycontract.cpp \
    factory_contract/findfactorycontract.cpp

HEADERS  += mainwindow.h \
    global.h \
    MysqlOperate/mysqloperate.h \
    MysqlOperate/mysqlstruct.h \
    MysqlOperate/mysqloperate.h \
    MysqlOperate/mysqlstruct.h \
    MysqlTableConfig/MysqlTableConfig.h \
    login.h \
    factory_contract/addfactorycontract.h \
    factory_contract/lookfactorycontract.h \
    factory_contract/findfactorycontract.h

FORMS    += mainwindow.ui \
    login.ui \
    login.ui \
    factory_contract/addfactorycontract.ui \
    factory_contract/lookfactorycontract.ui \
    factory_contract/findfactorycontract.ui
