1.#include <ros/ros.h>  
2.#include <std_msgs/Float32.h>  
3.#include <geometry_msgs/PoseWithCovarianceStamped.h>  
4.#include <string.h>  
5.#include "location.h"  
6.  
7.void scoutCallback(const geometry_msgs::PoseWithCovarianceStamped &msg)  
8.{  
9.    ros::NodeHandle nh;  
10.    nh.setParam("location_x", msg.pose.pose.position.x);  
11.    nh.setParam("location_y", msg.pose.pose.position.y);  
12.    nh.setParam("location_w", msg.pose.pose.orientation.w);      
13.    return;  
14.}  
15.  
16.int main(int argc, char** argv)  
17.{  
18.    ros::init(argc, argv, "location_scouts");  
19.    ros::NodeHandle Nh;  
20.    ros::Subscriber sub = Nh.subscribe("amcl_pose", 2, scoutCallback);  
21.    ros::spin();  
22.    return 0;  
23.}  
