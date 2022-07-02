1.#include <ros/ros.h>  
2.#include <dashgo_location/applicate.h>  
3.#include <std_msgs/Float32.h>  
4.#include <string.h>  
5.  
6.int main(int argc, char** argv)  
7.{  
8.        ros::init(argc, argv, "location_client"); // The node name is location_client.  
9.        if (argc != 2){  
10.                ROS_INFO("Usage: <location_name> \n Retry!\n");  
11.                return 1;  
12.        }  
13.        ros::NodeHandle Nh;  
14.        ros::ServiceClient client = Nh.serviceClient<dashgo_location::applicate>("location_manager"); // Create a new client, service name: location_manager  
15.        // Create a new srv, type: dashgo_location::applicate  
16.        dashgo_location::applicate srv;  
17.        srv.request.name = argv[1];  
18.        if (client.call(srv)){  
19.                ROS_INFO("The record is saved successfully!!");  
20.        }  
21.    else{  
22.        ROS_ERROR("Fail to record the location in the database!\n");  
23.                return 1;  
24.    }  
25.        return 0;  
26.} 
