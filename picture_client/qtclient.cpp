#include "qtclient.h"

QtClient::QtClient(QObject *parent) : QObject(parent)
{

    tcpClient.abort();                 //ȡ��ԭ������
    //connect(tcpClient, SIGNAL(readyRead()), this, SLOT(ReadData()));
    connect(&tcpClient, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(ReadError(QAbstractSocket::SocketError)));
    Success = false;
}


bool QtClient::Connect(){
    Success = false;
    tcpClient.connectToHost("47.94.219.135",8888);
    if (tcpClient.waitForConnected(3000)) return true;
    return false;
}
#include "msg_def.h"
#include <iostream>
#include <QFile>
using namespace std;
void QtClient::ReadData()
{
    qDebug() << "ReadDataReadData";
    int headlen = HEADLEN;
    Head h;
    {
        char head[HEADLEN];
        int readlen = 0;
        while (readlen < headlen) {
            tcpClient.waitForReadyRead(5000);
            int nowreadlen = tcpClient.read((char*)&head + readlen , headlen - readlen);
            readlen += nowreadlen;
            if (nowreadlen <= 0) {
                tcpClient.close();
                return;
            }
        }
        memcpy(&h, head, headlen);
        if (string(h.identify_code,38) != identifying_code) {
            tcpClient.close();
            return;
        }
        if (h.image_len < 0 || h.image_len > 10000000) {
            tcpClient.close();
            return;
        }
    }

    switch (h.msg_type) {
    case e_msg_type::download_return:
    {
        char *body = new char[h.image_len];
        int readlen = 0;
        while (readlen < h.image_len) {
            tcpClient.waitForReadyRead(5000);
            int nowreadlen = tcpClient.read( body+readlen , h.image_len - readlen);
            readlen += nowreadlen;
            if (nowreadlen <= 0) {
                tcpClient.close();
                delete []body;
                return;
            }
            tcpClient.waitForReadyRead(5000);
        }
        string imagedata = string(body,h.image_len);
        delete []body;
        uuid = QString::fromStdString(string(h.image_uuid,38));

        QFile file(save_dir + uuid);
        if( !file.open(QIODevice::WriteOnly) ){
            return;
        }
        file.write(imagedata.c_str(),h.image_len);
        file.close();

        Success = true;
        break;
    }
    case e_msg_type::upload_return:
        uuid = QString::fromStdString(string(h.image_uuid,38));
        Success = true;
        break;
    case e_msg_type::delete_picture_return:
        Success = true;
        break;
    default:
        tcpClient.disconnectFromHost();
        tcpClient.close();
        return;
    }
    tcpClient.disconnectFromHost();
    tcpClient.close();
}
#include <QFile>
#include <QDataStream>
void QtClient::UploadData(QString path){
    if( !Connect()) return ;
    QFile file(path);
    if( !file.open(QIODevice::ReadOnly) ){
        return;
    }
    QByteArray data = file.readAll();
    file.close();
    //�Ѵӱ��ض�ȡͼƬ����
    Head h_send;
    h_send.image_len = data.length();
    h_send.msg_type = e_msg_type::upload;
    qDebug()<< tcpClient.write((char*)&h_send, HEADLEN);
    qDebug()<< tcpClient.write(data);
    qDebug() << "wait for read data";
    ReadData();
}

void QtClient::DownData(QString uuid){
    if( !Connect()) return ;
    Head h_send;
    h_send.image_len = 0;
    memcpy( h_send.image_uuid,uuid.toStdString().c_str(),38);
    h_send.msg_type = e_msg_type::download;
    tcpClient.write((char*)&h_send, HEADLEN);
    ReadData();
}
#include <windows.h>
void QtClient::DeleteData(QString uuid){
    if( !Connect()) return ;
    Head h_send;
    h_send.image_len = 0;
    memcpy( h_send.image_uuid,uuid.toStdString().c_str(),38);
    h_send.msg_type = e_msg_type::delete_picture;
    tcpClient.write((char*)&h_send, HEADLEN);
    ReadData();
}


void QtClient::ReadError(QAbstractSocket::SocketError)
{
    tcpClient.disconnectFromHost();
}
