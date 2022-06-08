#include "twistudp.h"

short TwistUDP::getAngle(){
  return this->converter_angle.angle;
}

short TwistUDP::getSpeed(){
  return this->converter_speed.speed;
}

geometry_msgs::Twist TwistUDP::getTwist(){
  geometry_msgs::Twist tw;
  this->linear.x = converter_speed.speed*cos((converter_angle.angle*pi)/180);
  this->linear.y = converter_speed.speed*sin(converter_angle.angle*pi/180);
  this->linear.z = 0;

  this->angular.x=0;
  this->angular.y=0;
  this->angular.z=0;

  tw.linear=this->linear;
  tw.angular=this->angular;

  return tw;
}

void TwistUDP::read(){
  QByteArray buffer;
  QHostAddress sender;
  quint16 senderPort;
  if(socket->hasPendingDatagrams()){
    buffer.fill(0,socket->pendingDatagramSize());
    socket->readDatagram(buffer.data(), buffer.size(),
                         &sender, &senderPort);
    clientAddress = sender;
    clientPort = senderPort;
    qDebug() << "Message from: " << sender.toString();
    qDebug() << "Message port: " << senderPort;
    qDebug();
    if(this->isTwistPackage(buffer)){
      QByteArray bb = getSubQByteArray(buffer,5,buffer.size()-1);
      qDebug()<<"bb"<<bb;
      converter_angle.data[0]=bb[0];
      converter_angle.data[1]=bb[1];

      converter_speed.data[0]=bb[2];
      converter_speed.data[1]=bb[3];
//      converter_angle.angle = 0;
//      converter_angle.angle = ((int)bb[0])<<24;
//      converter_angle.angle = ((int)bb[1])<<16;
//      converter_angle.angle = ((int)bb[2])<<8;
//      converter_angle.angle = ((int)bb[3])<<0;

//      converter_speed.speed = ((int)bb[4])<<24;
//      converter_speed.speed = ((int)bb[5])<<16;
//      converter_speed.speed = ((int)bb[6])<<8;
//      converter_speed.speed = ((int)bb[7])<<0;

//      for(int i = 5+sizeof(int64_t), j = 0; i<5+2*sizeof(int64_t);i++, j++){
//        this->converter_speed.data[j]=buffer.data()[i];
//      }

      qDebug()<<"angle "<<converter_angle.angle;
      qDebug()<<"speed "<<converter_speed.speed;
    }else{
      converter_angle.angle=0;
      converter_speed.speed=0;
    }
  }
}

QByteArray TwistUDP::getSubQByteArray(QByteArray &data, int start, int finish){
  QByteArray bArr;
  for(int i = start; i<=finish; i++){
    bArr.append(data[i]);
  }
  return bArr;
}

bool TwistUDP::isTwistPackage(QByteArray data){
  QString temp = QString(this->getSubQByteArray(data,0,4));
  qDebug()<<temp;
  return !temp.compare("twist");
}

