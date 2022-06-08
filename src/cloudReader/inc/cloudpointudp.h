#ifndef CLOUDPOINTUDP_H
#define CLOUDPOINTUDP_H
#include <QObject>
#include <QUdpSocket>
#include <QTimer>
#include <QString>
#include <QThread>
#include <QTextCodec>
#include <QNetworkDatagram>
//#include <QNetworkDatagram>
#include <iostream>
using namespace std;

class CloudPointUDP: public QObject
{
  Q_OBJECT
public:
  explicit  CloudPointUDP(QObject *parent = 0 ):QObject(parent){
    socket = new QUdpSocket(this);
    if (socket->state() != socket->BoundState)
    {
      //bool res = socket->bind(QHostAddress("192.168.88.118"),port);
      bool res = socket->bind(QHostAddress("192.168.0.104"), port);//home
      //bool res = socket->bind(QHostAddress("127.0.0.1"),port);
      if(res)
      {
        qDebug() << "PASS";
        qDebug()<< socket->localAddress().toString();
        qDebug()<< socket->localPort();
        connect(socket, &QUdpSocket::readyRead,this, &CloudPointUDP::Read);
      }
      else
      {
        qDebug() << "FAIL";
      }
    }
  };
  
  static const int sizePackage =1455 ;
  ~CloudPointUDP(){};
  void setData(vector<QByteArray>& data);
  void setData(QByteArray data);
  void sendPacket(QByteArray& data);
  void sendPacket(vector<QByteArray>& data);
  
private:
  vector<QByteArray> data;
  QHostAddress clientAddress;
  QString serverAddress="192.168.0.104";
  int port=50001;
  int clientPort= 21333;
  QUdpSocket *socket;
  
public slots:
  void Read();
  void ReadTest();
  
};

#endif // CLOUDPOINTUDP_H
