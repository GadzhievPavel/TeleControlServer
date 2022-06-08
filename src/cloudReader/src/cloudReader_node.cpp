#include <ros/ros.h>
#include <sensor_msgs/PointCloud2.h>
#include <geometry_msgs/Point.h>
#include "pointcloud2translate.h"
#include <vector>
#include <iostream>
#include <QThread>
#include <QByteArray>
#include <cloudpointudp.h>
#include <QCoreApplication>
#include <stdio.h>
#include <pcl/point_types.h>
#include <pcl/filters/voxel_grid.h>
#include <pcl_conversions/pcl_conversions.h>
#include <pcl/point_cloud.h>
#include <iostream>
#include <fstream>
using namespace std;
static sensor_msgs::PointCloud2 currentCloud;
bool isNewData;
//Поток сокета
QThread *control_thread;
CloudPointUDP* cpUDP;

union convert{
  char data[2];
  int16_t i;
}convert;

void pointCloud2Callback(const sensor_msgs::PointCloud2 &msg)
{
  pcl::PointCloud<pcl::PointXYZ> cloud;
  pcl::PointCloud<pcl::PointXYZ> cloud_downsampled;
  sensor_msgs::PointCloud2 output;

  pcl::fromROSMsg(msg, cloud);

  pcl::VoxelGrid<pcl::PointXYZ> voxelSampler;
  voxelSampler.setInputCloud(cloud.makeShared());
  voxelSampler.setLeafSize(0.05f, 0.05f, 0.05f);
  voxelSampler.filter(cloud_downsampled);

  pcl::toROSMsg(cloud_downsampled, currentCloud);

  qDebug()<<"input"<<msg.data.size();
  qDebug()<<"filter"<<currentCloud.data.size();
  //currentCloud = msg;
  isNewData = true;
}

void pixelTo3DPoint(const sensor_msgs::PointCloud2& pCloud, const int u, const int v, vector<geometry_msgs::Point> &p)
{
  //Вычисляем начало позиции элемента
  geometry_msgs::Point point;
  int arrayPosition = v*pCloud.row_step + u*pCloud.point_step;
  // Вычисляем сдвиг каждого из полей
  int arrayPosX = arrayPosition + pCloud.fields[0].offset; // X имеет сдвиг 0
  int arrayPosY = arrayPosition + pCloud.fields[1].offset; // Y имеет сдвиг 4
  int arrayPosZ = arrayPosition + pCloud.fields[2].offset; // Z имеет сдвиг 8
  float X = 0.0;
  float Y = 0.0;
  float Z = 0.0;
  memcpy(&X, &pCloud.data[arrayPosX], sizeof(float));
  memcpy(&Y, &pCloud.data[arrayPosY], sizeof(float));
  memcpy(&Z, &pCloud.data[arrayPosZ], sizeof(float));
  point.x = X;
  point.y = Y;
  point.z = Z;
  p.push_back(point);

}

void printFile(float X, float Y, float Z){
  ofstream out;
  out.open("/home/pavel/TeleControlServer/pc.txt", ios::app);
  if(out.is_open()){
    char x[8];
    char y[8];
    char z[8];
    int ret;
    out<<"float X ";
    ret = snprintf(x, sizeof(x), "%f", X);
    if(ret>0){
      out<<x;
    }
    out<<" Y ";
    ret = snprintf(y, sizeof(y), "%f", Y);
    if(ret>0){
      out<<y;
    }
    out<<" Z ";
    ret = snprintf(z, sizeof(z), "%f", Z);
    if(ret>0){
      out<<z;
    }

    int16_t x16 = (int16_t)(X*(float)100);
    int16_t y16 = (int16_t)(Y*(float)100);
    int16_t z16 = (int16_t)(Z*(float)100);

    out<<" int16_t X ";
    ret = snprintf(x, sizeof(x), "%d", x16);
    if(ret>0){
      out<<x;
    }
    out<<" Y ";
    ret = snprintf(y, sizeof(y), "%d", y16);
    if(ret>0){
      out<<y;
    }
    out<<" Z ";
    ret = snprintf(z, sizeof(z), "%d", z16);
    if(ret>0){
      out<<z;
    }

    QByteArray xByte, yByte, zByte;

    convert.i=x16;
    xByte.append(convert.data[0]);
    xByte.append(convert.data[1]);

    convert.i=y16;
    yByte.append(convert.data[0]);
    yByte.append(convert.data[1]);

    convert.i=z16;
    zByte.append(convert.data[0]);
    zByte.append(convert.data[1]);

    out<<" qByteArray X ";
    for(int i =0; i<xByte.size();i++){
      out<<xByte.at(i);
    }
    out<<" Y ";
    for(int i =0; i<yByte.size();i++){
      out<<yByte.at(i);
    }

    out<<" Z ";
    for(int i =0; i<zByte.size();i++){
      out<<zByte.at(i);
    }


    out<<endl;
    out.close();
  }
  //  file.write("int16_t X");
  //  //file.write((int16_t)(X*(float)100));
  //  file.write("Y");
  //  //file.write((int16_t)(Y*(float)100));
  //  file.write("Z");
  //  //file.write((int16_t)(Z*(float)100));
  //  file.write("QByte X");
  //  file.close();
}

void pixelToByteArrays(const sensor_msgs::PointCloud2 &pCloud, const int u, const int v, QByteArray &arr, vector<QByteArray>&vectorArr){
  int arrayPosition = v*pCloud.row_step + u*pCloud.point_step;
  int arrayPosX = arrayPosition + pCloud.fields[0].offset; // X имеет сдвиг 0
  int arrayPosY = arrayPosition + pCloud.fields[1].offset; // Y имеет сдвиг 4
  int arrayPosZ = arrayPosition + pCloud.fields[2].offset; // Z имеет сдвиг 8
  float X = 0.0;
  float Y = 0.0;
  float Z = 0.0;
  memcpy(&X, &pCloud.data[arrayPosX], sizeof(float));
  memcpy(&Y, &pCloud.data[arrayPosY], sizeof(float));
  memcpy(&Z, &pCloud.data[arrayPosZ], sizeof(float));
  if(arr.size()+3*2>CloudPointUDP::sizePackage){
//    arr.push_front(QByteArray::number((int)vectorArr.size()));
//    arr.push_front("n");
    //qDebug()<<"arr"<<sizeof(arr.at(0))*arr.size();
    vectorArr.push_back(arr);
    arr.clear();
  }
  printFile(X,Y,Z);
  //add X
//  arr.append(((int16_t)(X*(float)100))& 0xff);
//  arr.append((((int16_t)(X*(float)100))& 0xff00) >> 8);
//  //add Y
//  arr.append(((int16_t)(Y*(float)100))& 0xff);
//  arr.append((((int16_t)(Y*(float)100))& 0xff00) >> 8);
//  //add Z
//  arr.append(((int16_t)(Z*(float)100))& 0xff);
//  arr.append((((int16_t)(Z*(float)100))& 0xff00) >> 8);
  convert.i = (int16_t)(X*(float)100);
  arr.append(convert.data[0]);
  arr.append(convert.data[1]);
  convert.i = (int16_t)(Y*(float)100);
  arr.append(convert.data[0]);
  arr.append(convert.data[1]);
  convert.i = (int16_t)(Z*(float)100);
  arr.append(convert.data[0]);
  arr.append(convert.data[1]);
}


void pixelToByteArray(const sensor_msgs::PointCloud2& pCloud, const int u, const int v, QByteArray& arr, vector<QByteArray>& vectorArr){
  int arrayPosition = v*pCloud.row_step + u*pCloud.point_step;
  // Вычисляем сдвиг каждого из полей
  int arrayPosX = arrayPosition + pCloud.fields[0].offset; // X имеет сдвиг 0
  int arrayPosY = arrayPosition + pCloud.fields[1].offset; // Y имеет сдвиг 4
  int arrayPosZ = arrayPosition + pCloud.fields[2].offset; // Z имеет сдвиг 8
  float X = 0.0;
  float Y = 0.0;
  float Z = 0.0;
  memcpy(&X, &pCloud.data[arrayPosX], sizeof(float));
  memcpy(&Y, &pCloud.data[arrayPosY], sizeof(float));
  memcpy(&Z, &pCloud.data[arrayPosZ], sizeof(float));
  char x[8];
  char y[8];
  char z[8];
  int ret;
  if(arr.size()>CloudPointUDP::sizePackage){
    arr.push_front(QByteArray::number((int)vectorArr.size()));
    arr.push_front("n");
    //qDebug()<<"arr"<<sizeof(arr.at(0))*arr.size();
    vectorArr.push_back(arr);
    arr.clear();
  }
  ret = snprintf(x, sizeof(x), "%f", X);
  if (ret >= sizeof(x)) {
    // Result was truncated - resize the buffer and retry.
    arr.append("x");
    arr.append(x);
  }
  ret = snprintf(y, sizeof(y), "%f", Y);
  if (ret >= sizeof(y)) {
    // Result was truncated - resize the buffer and retry.
    arr.append("y");
    arr.append(y);
  }

  ret = snprintf(z, sizeof(z), "%f", Z);
  if (ret >= sizeof(z)) {
    // Result was truncated - resize the buffer and retry.
    arr.append("z");
    arr.append(z);
  }
}



int main(int argc, char **argv)
{
  ros::init(argc, argv, "cloudReader_node");
  ros::NodeHandle nh;
  ros::Publisher pub_cloud = nh.advertise<sensor_msgs::PointCloud2>("pcl_cloud",100);
  ros::Subscriber cloud_sub = nh.subscribe("/camera/depth/points", 10, pointCloud2Callback);
  QCoreApplication app (argc,argv);
  control_thread = new QThread();
  cpUDP = new CloudPointUDP();
  cpUDP->moveToThread(control_thread);
  ofstream out;
  out.open("/home/pavel/TeleControlServer/pc.txt");
  out.close();
  control_thread->start();
  isNewData = false;
  ros::Rate r(10);
  int count = 0;
  while(ros::ok()) {
    if(isNewData) {
      cout<<"point cloud"<< count++<<endl;
      QByteArray qbd;
      QByteArray testqbd;
      vector<QByteArray> vectorByteArray;
      //qbd.append("new data");
      for(int i = 0; i < currentCloud.width; i++) {
        for(int j = 0; j < currentCloud.height; j++) {
          //pixelToByteArray(currentCloud,i,j,qbd,vectorByteArray);
          pixelToByteArrays(currentCloud,i,j,testqbd,vectorByteArray);
        }
      }
      cout<<"size qbyteArray string"<<qbd.size()*vectorByteArray.size()*sizeof(qbd.at(0))<<endl;
      cout<<"size qbyteArray int16_t"<<testqbd.size()*sizeof(testqbd.at(0))<<endl;
      currentCloud.header.frame_id="glob";
      pub_cloud.publish(currentCloud);
      //cout<<currentCloud.data.size();
      cpUDP->setData(vectorByteArray);
      isNewData = false;
    }
    ros::spinOnce();
  }
}
