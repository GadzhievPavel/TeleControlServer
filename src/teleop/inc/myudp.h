#ifndef MYUDP_H
#define MYUDP_H

#include <QUdpSocket>
#include <QTimer>
#include <QString>
#include <QTextCodec>
//#include <QNetworkDatagram>
#include <iostream>

using namespace std;

class MyUDP : public QObject
{
//  Q_OBJECT
public:
  explicit MyUDP(QObject *parent = 0);
  void HelloUDP();
  int getSpeed();
  int getAngle();
signals:

public slots:
  void readyRead();
  void sendPacket(int data);

private:
    QString clientAddress="192.168.1.36";
    QString serverAddress="192.168.1.254";
    int port=21321;
    int val;
    int speed, angle;
    QUdpSocket *socket;
    bool isStartPacket(QByteArray buffer);
};

#endif // MYUDP_H
