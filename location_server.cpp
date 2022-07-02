#include <ros/ros.h>  
#include "location.h"  
#include <dashgo_location/applicate.h>  
#include <string.h>  
#include <std_msgs/Float32.h>  
#include <jsoncpp/json/json.h>  
#include <iostream>  
#include <fstream>  
using namespace std;  
  
bool requesting_func(dashgo_location::applicate::Request &req, dashgo_location::applicate::Response &res)  
{  
        printf("Receiving a request:\n        location name:%s\n", req.name.c_str());  
        ofstream out_channel;  
    ifstream in_channel;  
  
    in_channel.open("/home/eaibot/dashgo_ws/src/dashgo/dashgo_location/location.json", ios::in);  
        if (!in_channel.is_open()){  
                cout << "Error: JSON file cannot be opened" << endl;  
                res.feedback = "Sorry, the saving fails.\n";  
                return false;  
        }  
    // Write in the JSON file.  
        Json::Value tri_coor;  
    double* value_list = new double[3000];  
    Json::Value root;  
    Json::Value root_write;  
    Json::Reader reader;  
        std_msgs::Float32 loc_x, loc_y, loc_w;  
    string* name_list = new string[1000];  
    string name;  
    int size;  
    if (reader.parse(in_channel, root)){  
        size = root["location"].size();  
        ROS_INFO("There are %d location existing.\n", size);  
        for (int i = 0; i < size; i++){  
            Json::Value loc = root["location"];  
            name_list[i] = loc[i]["Name"].asString();  
            loc_x.data = loc[i]["x"].asDouble();  
            loc_y.data = loc[i]["y"].asDouble();  
            loc_w.data = loc[i]["w"].asDouble();  
            value_list[i*3] = loc_x.data;  
                value_list[i*3+1] = loc_y.data;  
                value_list[i*3+2] = loc_w.data;  
        }  
    }  
    in_channel.close();  
      
    out_channel.open("/home/eaibot/dashgo_ws/src/dashgo/dashgo_location/location.json", ios::out);    
        if (!out_channel.is_open()){  
                cout << "Error: JSON file cannot be opened" << endl;  
                res.feedback = "Sorry, the saving fails.\n";  
                return false;  
        }  
    for (int i = 0; i<size; i++){  
        tri_coor["x"] = value_list[i*3];  
        tri_coor["y"] = value_list[i*3+1];  
        tri_coor["w"] = value_list[i*3+2];  
        tri_coor["Name"] = name_list[i];   
        root_write["location"].append(tri_coor);  
    }  
        ros::NodeHandle nh;  
    nh.getParam("location_x", loc_x.data);  
    nh.getParam("location_y", loc_y.data);  
    nh.getParam("location_w", loc_w.data);  
    ROS_INFO("The location is:\n%f\n%f\n%f\n", loc_x.data, loc_y.data, loc_w.data);  
        tri_coor["x"] = loc_x.data;  
        tri_coor["y"] = loc_y.data;  
        tri_coor["w"] = loc_w.data;  
    tri_coor["Name"] = req.name;  
    root_write["location"].append(tri_coor);  
  
        Json::StyledWriter sw;  
        out_channel << sw.write(root_write) << endl;  
        out_channel.close();  
  
        res.feedback = "The record saves successdfully.\n";  
        return true;  
}  

int main(int argc, char** argv)  
{  
        ros::init(argc, argv, "location_server");  
        ros::NodeHandle Nh;  
    ROS_INFO("The location_server has started!");  
        ros::ServiceServer service = Nh.advertiseService("location_manager", requesting_func);  
        ros::spin();  
        return 0;  
} 
