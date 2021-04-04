#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <geometry_msgs/Vector3.h>
#include "myudp.h"

geometry_msgs::Vector3 angular;
geometry_msgs::Vector3 linear;

geometry_msgs::Vector3 getLinearTwist(int angle, int speed)
{
  linear.x = speed*cos(angle)/100;
  linear.y = speed*sin(angle)/100;
  linear.z = 0;
  return linear;
}

geometry_msgs::Vector3 getAngularTwist()
{
  angular.x = 0;
  angular.y = 0;
  angular.z = 0;
  return angular;
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "teleop_node");
  ros::NodeHandle nh;
  ros::Publisher pub = nh.advertise<geometry_msgs::Twist>("command",1000);
  ros::Rate r(100);
  MyUDP client;
  while (ros::ok()) {
    client.readyRead();
    //client.sendPacket(client.getAngle());
    geometry_msgs::Twist twist;
    twist.angular=getAngularTwist();
    twist.linear=getLinearTwist(client.getAngle(),client.getSpeed());
    pub.publish(twist);
    ros::spinOnce();
    r.sleep();
  }
}
