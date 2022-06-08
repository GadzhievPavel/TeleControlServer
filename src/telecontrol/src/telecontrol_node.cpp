#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <geometry_msgs/Vector3.h>
#include "twistudp.h"
#include <QCoreApplication>
int main(int argc, char **argv)
{
  ros::init(argc, argv, "telecontrol_node");
  ros::NodeHandle nh;

  QThread *control_thread = new QThread();
  QCoreApplication app (argc,argv);

  ros::Publisher pub = nh.advertise<geometry_msgs::Twist>("control",1000);
  TwistUDP *twistUDP = new TwistUDP();
  twistUDP->moveToThread(control_thread);
  control_thread->start();
  ros::Rate r(100);
  while (ros::ok()) {
    geometry_msgs::Twist twist = twistUDP->getTwist();
    pub.publish(twist);
    ros::spinOnce();
    r.sleep();
  }
}
