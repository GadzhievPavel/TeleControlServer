#ifndef TWISTUDP_H
#define TWISTUDP_H
#include <QObject>
#include <QUdpSocket>
#include <QThread>
#include <QTextCodec>
#include <QNetworkDatagram>
#include <geometry_msgs/Twist.h>
#include <geometry_msgs/Vector3.h>
class TwistUDP : public QObject{
  Q_OBJECT
private:
  int port=50002;
  int clientPort= 21333;
  QUdpSocket *socket;
  QHostAddress clientAddress;
  QString serverAddress="192.168.0.104";
  //QString serverAddress="192.168.88.118";
  geometry_msgs::Vector3 angular;
  geometry_msgs::Vector3 linear;
  const double pi = 3.14159265;
  union converterAngle{
    char data[2];
    short int angle;
  }converter_angle;

  union converterSpeed{
    char data[2];
    short int speed;
  }converter_speed;

  bool isTwistPackage(QByteArray data);
  QByteArray getSubQByteArray(QByteArray & data,int start,int finish);

public:
  explicit  TwistUDP(QObject *parent = 0 ):QObject(parent){
    socket = new QUdpSocket(this);
    if (socket->state() != socket->BoundState)
    {
      converter_angle.angle = 0;
      converter_speed.speed = 0;
      bool res = socket->bind(QHostAddress(serverAddress), port);//home
      if(res)
      {
        qDebug() << "PASS";
        qDebug()<< socket->localAddress().toString();
        qDebug()<< socket->localPort();
        connect(socket, &QUdpSocket::readyRead,this, &TwistUDP::read);
      }
      else
      {
        qDebug() << "FAIL";
      }
    }
  };

  short getAngle();
  short getSpeed();
  geometry_msgs::Twist getTwist();

public slots:
  void read();
};

#endif // TWISTUDP_H
