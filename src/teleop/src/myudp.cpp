#include "myudp.h"

MyUDP::MyUDP(QObject *parent) : QObject(parent)
{
  socket = new QUdpSocket(this);
  socket->bind(QHostAddress(serverAddress), port);
  connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
}

bool MyUDP::isStartPacket(QByteArray buffer){
  bool flag = true;
  if(buffer.size()==5){
    if(buffer.at(0)=='s' && buffer.at(1)=='t' &&
       buffer.at(2)=='a' && buffer.at(3)=='r' && buffer.at(4)=='t'){
        flag = false;
    }
  }
  return flag;
}

void MyUDP::readyRead(){
  QByteArray buffer;
  buffer.resize(socket->pendingDatagramSize());
  QHostAddress sender;
  quint16 senderPort;
  socket->readDatagram(buffer.data(), buffer.size(),
                           &sender, &senderPort);
  if(MyUDP::isStartPacket(buffer)){
    MyUDP::clientAddress = sender.toString();
    qDebug() << "client have address "<< MyUDP::clientAddress;
  }

  if(sender.toString()==MyUDP::clientAddress){
    qDebug() << "Message from: " << sender.toString();
    qDebug() << "Message port: " << senderPort;
    unsigned char arrayTemp[4]={0,0,0,0};
    for(int i=0; i<buffer.size();i++){
        qDebug() << "Data["<<i<<"] "<<(int) (buffer.at(i));
        arrayTemp[i]=buffer.at(i);
    }
//    if(buffer.size()==5){
//      if(buffer.at(3)<0){
//        MyUDP::angle=256+buffer.at(3);
//      }else{
//        MyUDP::angle=buffer.at(3)+buffer.at(2)*256;
//      }
//      MyUDP::speed=buffer.at(4);
//      qDebug() << "Message angle "<<MyUDP::angle;
//      //MyUDP::sendPacket(MyUDP::angle);
//    }
  }
}

void MyUDP::sendPacket(int data){
  QByteArray dataSend;
  dataSend.append(100);
  dataSend.append(100);
  dataSend.append(100);
  dataSend.append(100);
  if(data>80 && data<100){
    qDebug() << "in func "<< data;
    dataSend[0]=1;
  }else{
    dataSend[0]=0;
  }
  socket->writeDatagram(dataSend,QHostAddress(clientAddress),55445);
}

int MyUDP::getAngle(){
  return MyUDP::angle;
}

int MyUDP::getSpeed(){
  return MyUDP::speed;
}


