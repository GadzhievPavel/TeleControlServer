#include "inc/cloudpointudp.h"

void CloudPointUDP::Read(){
  QByteArray buffer;
  QHostAddress sender;
  quint16 senderPort;
  while (!socket->hasPendingDatagrams()) {
  }
  if(socket->hasPendingDatagrams()){
    buffer.fill(0,socket->pendingDatagramSize());
    socket->readDatagram(buffer.data(), buffer.size(),
                         &sender, &senderPort);
    clientAddress = sender;
    clientPort = senderPort;
    qDebug() << "Message from: " << sender.toString();
    qDebug() << "Message port: " << senderPort;

    QString temp{buffer.data()};
    qDebug();
    if(!temp.compare("pc")){
      qDebug()<<"send";
      CloudPointUDP::sendPacket(data);
    }
  }
}

void CloudPointUDP::ReadTest(){
  qDebug()<<"test init";
  while (socket->hasPendingDatagrams()) {
         QNetworkDatagram datagram = socket->receiveDatagram();
         qDebug()<<" test";
     }
}
void CloudPointUDP::sendPacket(vector<QByteArray>& _data){
  int size = _data.size();
  int lose = 0;
  for(int i = 0; i< _data.size(); i++){
    int req = socket->writeDatagram(_data.at(i),clientAddress,clientPort);
    if(req == -1){
      lose++;
    }
  }
  qDebug()<<"lose"<<lose<<"is"<<size;

}

//void CloudPointUDP::sendPacket(QByteArray& data){
//  qDebug("start send");
//  qDebug()<<"send to "<<clientAddress;
//  int i = socket->writeDatagram(data,QHostAddress(clientAddress),clientPort);
//  qDebug()<<"send data"<<i;
//}

void CloudPointUDP::setData(vector<QByteArray>& data){
  QByteArray end{"end"};
  QString s = QString::number(data.size());
  qDebug()<<"count package"<<s;
  QByteArray str{"s"};

  str+= s.toUtf8();
  this->data.clear();
  this->data.push_back(str);
  this->data.insert(this->data.end(),data.begin(),data.end());
  this->data.push_back(end);

}

void CloudPointUDP::setData(QByteArray data){
  this->data.push_back(data);
}
