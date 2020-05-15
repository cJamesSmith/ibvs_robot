#include <string>
#include <ros/ros.h>
#include <iostream>
#include <time.h>
#include "vector"
#include "std_msgs/Float32.h"
#include "std_msgs/Float64MultiArray.h"
#include "controller_manager_msgs/SwitchController.h"
#include "controller_manager_msgs/ListControllers.h"
#include "my_ikpk.hpp"
#include <Eigen/Eigen>

int main(int argc, char **argv)
{
    ros::init(argc, argv, "mission_1");

    double link[6] = {0.322,-0.855,-0.021,0,0.877,0.322};

    calPKPos(link);
    std::vector<double>  link_pos;
    
    std::vector<double> end_pos = {0.15, 0.2, 0.2007, 0, 0, 0};

    link_pos = myCalIK(end_pos);

    ros::NodeHandle node_handle;
    ros::AsyncSpinner spinner(1);
    ROS_INFO_STREAM("start");

    ros::Publisher pos_pub = node_handle.advertise<std_msgs::Float64MultiArray>("/probot_anno/arm_pos_controller/command", 100);

    std_msgs::Float64MultiArray target_pos;
    for(int i = 0; i<6; i++){
        target_pos.data.push_back(0);
    }
    sleep(1);

    // for(int i = 0; i< 6 ;i++){
    //     target_pos.data.at(i) = link_pos[i];
    // }
    target_pos.data.at(0) = 0.322;
    target_pos.data.at(1) = -0.855;
    target_pos.data.at(2) = -0.021;
    target_pos.data.at(3) = 0;
    target_pos.data.at(4) = 0.877;
    target_pos.data.at(5) = 0.322;

    pos_pub.publish(target_pos);
    ROS_INFO_STREAM("published");
}