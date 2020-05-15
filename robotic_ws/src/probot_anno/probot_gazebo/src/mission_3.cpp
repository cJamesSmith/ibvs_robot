#include <string>
#include <ros/ros.h>
#include <iostream>
#include <time.h>
#include "vector"
#include "std_msgs/String.h"
#include "gazebo_msgs/LinkStates.h"
#include "std_msgs/Float32.h"
#include "std_msgs/Float64MultiArray.h"
#include "controller_manager_msgs/SwitchController.h"
#include "controller_manager_msgs/ListControllers.h"
#include "my_ikpk.hpp"
#include "jacobi.hpp"

ros::Time t;
double inBegin;

std::vector<double> calRoute(double endT, double startPos, double endPos, double startVel, double endVel, double startAcc, double endAcc);

class SubscribeAndPublish
{
public:
    SubscribeAndPublish()
    {
        for (int i = 0; i < 6; i++)
        {
            init_pos.data.push_back(0);
        }
        pub_ = n_.advertise<std_msgs::Float64MultiArray>("/probot_anno/arm_vel_controller/command", 100);
        sub_ = n_.subscribe("/gazebo/link_states", 100, &SubscribeAndPublish::poseCallBack, this);
    }
    void poseCallBack(const gazebo_msgs::LinkStatesConstPtr &msg)
    {
        for (int i = 2; i < 8; i++)
        {
            pos[i - 2].x = msg->pose[i].position.x;
            pos[i - 2].y = msg->pose[i].position.y;
            pos[i - 2].z = msg->pose[i].position.z;
            orientation[i-2].x = msg->pose[i].orientation.x;
            orientation[i-2].y = msg->pose[i].orientation.y;
            orientation[i-2].z = msg->pose[i].orientation.z;
            orientation[i-2].w = msg->pose[i].orientation.w;
            vel[i-2].x = msg->twist[i].linear.x;
            vel[i-2].y = msg->twist[i].linear.y;
            vel[i-2].z = msg->twist[i].linear.z;
        }
        std::cout << "link_pos 6"<< " : (" <<pos[5].x<< " ," <<pos[5].y<< " ," <<pos[5].z<<") "<<std::endl;
        std::cout << "link_orientation 6 : (" << orientation[5].x << " ,"<< orientation[5].y << " ,"<< orientation[5].z << " ,"<< orientation[5].w << std::endl;
        std::cout << "link_vel 6"<< " : (" <<vel[5].x<< " ," <<vel[5].y<< " ," <<vel[5].z<<") "<<std::endl;
    }

private:
    ros::NodeHandle n_;
    ros::Publisher pub_;
    ros::Subscriber sub_;
    geometry_msgs::Point pos[6];
    geometry_msgs::Quaternion orientation[6];
    geometry_msgs::Vector3 vel[6];
    double currentVel = 0;
    std_msgs::Float64MultiArray init_pos;
};

int main(int argc, char **argv)
{
    ros::init(argc, argv, "mission_1");

    ros::NodeHandle node_handle;
    ros::AsyncSpinner spinner(1);
    SubscribeAndPublish SAPObject;
    ROS_INFO_STREAM("start");
    t = ros::Time(0);
    ros::Publisher pos_pub = node_handle.advertise<std_msgs::Float64MultiArray>("/probot_anno/arm_vel_controller/command", 100);

    // double routePos[6][4] = {{0, 0.795499, 0.387524, -0.387524},
    //                          {0, -0.772855, -1.07552, -1.29294},
    //                          {0, 0.205023, -0.199728, -0.0654229},
    //                          {0, -2.05585, -2.73831, 2.74605},
    //                          {0, 0.631168, 0.273048, 0.196463},
    //                          {0, 2.29983, 3.02704, -3.06027}};
    // double routeVel[6][4] = {
    //     {0, 0.3, 0.3, 0},
    //     {0, 0.3, 0.3, 0},
    //     {0, 0.3, 0.3, 0},
    //     {0, 0.3, 0.3, 0},
    //     {0, 0.3, 0.3, 0},
    //     {0, 0.3, 0.3, 0},
    // };
    // double routeAcc[6][4] = {
    //     {0, 0.05, 0.05, 0},
    //     {0, 0.05, 0.05, 0},
    //     {0, 0.05, 0.05, 0},
    //     {0, 0.05, 0.05, 0},
    //     {0, 0.05, 0.05, 0},
    //     {0, 0.05, 0.05, 0},
    // };
    // double endT = 5;
    // t = ros::Time(0);
    // do
    // {
    //     ros::Time now_1 = ros::Time::now();
    //     inBegin = now_1.toSec() - t.toSec();
    // } while (inBegin == 0);

    // std::vector<std::vector<double>> a;
    // a.resize(6);
    // double result_vel[6];

    // std_msgs::Float64MultiArray target_pos;
    // for (int i = 0; i < 6; i++)
    // {
    //     target_pos.data.push_back(0);
    // }

    // for (int i = 0; i < 3; i++)
    // {
    //     ros::Time now = ros::Time::now();
    //     double duration = (now.toSec() - t.toSec()) - inBegin;

    //     while (duration <= endT)
    //     {
    //         ros::Time now_2 = ros::Time::now();
    //         duration = (now_2.toSec() - t.toSec()) - inBegin;
    //         for (int j = 0; j < 6; j++)
    //         {
    //             a[j] = calRoute(endT, routePos[j][i], routePos[j][i + 1],
    //                             routeVel[j][i], routeVel[j][i + 1], routeAcc[j][i], routeAcc[j][i + 1]);
    //         }
    //         for (int k = 0; k < 6; k++)
    //         {
    //             result_vel[k] = a[k][1] + 2 * a[k][2] * duration + 3 * a[k][3] * duration * duration +
    //                             4 * a[k][4] * duration * duration * duration +
    //                             5 * a[k][5] * duration * duration * duration * duration;
    //         }
    //         for (int i = 0; i < 6; i++)
    //         {
    //             target_pos.data.at(i) = result_vel[i];
    //         }
    //         pos_pub.publish(target_pos);
    //         // std::cout << "times : " << duration << std::endl;
    //     }
    //     ros::Time now_3 = ros::Time::now();
    //     inBegin = now_3.toSec() - t.toSec();
    // }
    double routePos[6][4] = {
        {0, 0.795499, 0.387524, -0.387524},
        {0, -0.772855, -1.07552, -1.29294},
        {0, 0.205023, -0.199728, -0.0654229},
        {0, -2.05585, -2.73831, 2.74605},
        {0, 0.631168, 0.273048, 0.196463},
        {0, 2.29983, 3.02704, -3.06027}};
    double routeVel[6][4] = {
        {0, 0.1, 0.05, 0},
        {0, 0.1, 0.05, 0},
        {0, 0.1, 0.05, 0},
        {0, 0.1, 0.05, 0},
        {0, 0.1, 0.05, 0},
        {0, 0.1, 0.05, 0}};
    double routeAcc[6][4] = {
        {0, 0.05, 0.05, 0},
        {0, 0.05, 0.05, 0},
        {0, 0.05, 0.05, 0},
        {0, 0.05, 0.05, 0},
        {0, 0.05, 0.05, 0},
        {0, 0.05, 0.05, 0}};
    double endT[3] = {6, 9, 14};
    t = ros::Time(0);
    do
    {
        ros::Time now_1 = ros::Time::now();
        inBegin = now_1.toSec() - t.toSec();
    } while (inBegin == 0);

    std::vector<std::vector<double>> a;
    a.resize(6);
    double result_vel[6];

    std_msgs::Float64MultiArray target_pos;
    for (int i = 0; i < 6; i++)
    {
        target_pos.data.push_back(0);
    }

    for (int i = 0; i < 3; i++)
    {
        ros::Time now = ros::Time::now();
        double duration = (now.toSec() - t.toSec()) - inBegin;

        while (duration <= endT[i])
        {
            ros::Time now_2 = ros::Time::now();
            duration = (now_2.toSec() - t.toSec()) - inBegin;
            for (int j = 0; j < 6; j++)
            {
                a[j] = calRoute(endT[i], routePos[j][i], routePos[j][i + 1],
                                routeVel[j][i], routeVel[j][i + 1], routeAcc[j][i], routeAcc[j][i + 1]);
            }
            for (int k = 0; k < 6; k++)
            {
                result_vel[k] = a[k][1] + 2 * a[k][2] * duration + 3 * a[k][3] * duration * duration +
                                4 * a[k][4] * duration * duration * duration +
                                5 * a[k][5] * duration * duration * duration * duration;
            }
            for (int i = 0; i < 6; i++)
            {
                target_pos.data.at(i) = result_vel[i];
            }
            pos_pub.publish(target_pos);
            // std::cout << "times : " << duration << std::endl;
        }
        ros::spinOnce();
        std::cout << "------------------------"<<std::endl;
        ros::Time now_3 = ros::Time::now();
        inBegin = now_3.toSec() - t.toSec();
    }
    for (int i = 0; i < 6; i++)
    {
        target_pos.data.at(i) = 0;
    }
    while (ros::ok())
    {
        pos_pub.publish(target_pos);
    }
}

std::vector<double> calRoute(double endT, double startPos, double endPos, double startVel, double endVel, double startAcc, double endAcc)
{
    std::vector<double> a;
    a.resize(6);
    a[0] = startPos;
    a[1] = startVel;
    a[2] = startAcc / 2;
    a[3] = (20 * endPos - 20 * startPos - (8 * endVel + 12 * startVel) * endT - (3 * startAcc - endAcc) * endT * endT) / (2 * endT * endT * endT);
    a[4] = (30 * startPos - 30 * endPos + (14 * endVel + 16 * startVel) * endT + (3 * startAcc - 2 * endAcc) * endT * endT) / (2 * endT * endT * endT * endT);
    a[5] = (12 * endPos - 12 * startPos - (6 * endVel + 6 * startVel) * endT - (startAcc - endAcc) * endT * endT) / (2 * endT * endT * endT * endT * endT);
    return a;
}