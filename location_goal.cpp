#include <ros/ros.h>  
#include <geometry_msgs/PoseStamped.h>  
#include <std_msgs/Float32.h>  
#include <jsoncpp/json/json.h>  
#include <string.h>  
#include <iostream>  
#include <fstream>  
using namespace std;  
  
int main(int argc, char** argv)  
{  
    ros::init(argc, argv, "location_goal");  
    if (argc != 2 && argc != 4){  
            ROS_INFO("Usage: <the location name> \nOR\n Usage: <x> <y> <w> \nRetry!\n");  
            return 1;  
    }  
    ros::NodeHandle Nh;  
    geometry_msgs::PoseStamped msg;  
    std_msgs::Float32 loc_x, loc_y, loc_w;  
    msg.header.frame_id = "map";  
    if (argc == 4){  
        loc_x.data = atof(argv[1]);  
        loc_y.data = atof(argv[2]);  
        loc_w.data = atof(argv[3]);  
        msg.pose.position.x = loc_x.data;  
        msg.pose.position.y = loc_y.data;  
        msg.pose.orientation.w = loc_w.data;  
    }  
    if (argc == 2){  
    Json::Reader reader;  
        Json::Value root;  
        if (argv[1] == "clear_all"){  
        ofstream os;  
        os.open("/home/eaibot/dashgo_ws/src/dashgo/dashgo_location/location.json", ios::out);  
            if (!os.is_open()){  
                ROS_INFO("Error opening file\n");  
                return 1;  
            }  
        os << "";  
        os.close();  
        return 0;  
    }  
  
    ifstream in;  
    in.open("/home/eaibot/dashgo_ws/src/dashgo/dashgo_location/location.json", ios::binary);  
        if (!in.is_open())  
        {  
            ROS_INFO("Error opening file\n");  
            return 1;  
        }  
        if (reader.parse(in, root)){  
        int size = root["location"].size();       
        string name = argv[1];  
        bool flag = false;  
        for (int i=0; i<size; i++){  
            if (root["location"][i]["Name"] == name){  
                    msg.pose.position.x = root["location"][i]["x"].asDouble();  
                        msg.pose.position.y = root["location"][i]["y"].asDouble();  
                    msg.pose.orientation.w = root["location"][i]["w"].asDouble();  
                        ROS_INFO("The location is : %f %f %f", msg.pose.position.x, msg.pose.position.y, msg.pose.orientation.w);  
                flag = true;  
                break;  
            }  
        }  
        if (!flag){  
            ROS_INFO("No such location recorded!\n");  
            return 1;  
        }         
    }  
    }  
    ros::Publisher pub = Nh.advertise<geometry_msgs::PoseStamped>("move_base_simple/goal", 1);  
    ros::Rate loop_rate(0.5);  
    while(ros::ok()){  
        ROS_INFO("Destination: x: %f  y: %f  w:%f\nPUblishing...Done!", loc_x.data, loc_y.data, loc_w.data);  
        pub.publish(msg);  
        loop_rate.sleep();  
    }  
    return 0;  
}  
