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

std::vector<double> calRoute(double endT, double startPos, double endPos, double startVel, double endVel, double startAcc, double endAcc);

class SubscribeAndPublish
{
public:
    SubscribeAndPublish()
    {
        sub_ = n_.subscribe("/gazebo/link_states", 100, &SubscribeAndPublish::poseCallBack, this);
    }
    void poseCallBack(const gazebo_msgs::LinkStatesConstPtr &msg)
    {
        for (int i = 0; i < 9; i++)
        {
            pos[i -2].x = msg->pose[i].position.x;
            pos[i -2 ].y = msg->pose[i].position.y;
            pos[i -2].z = msg->pose[i].position.z;
            std::cout << "link_pos "<< i << " : (" <<pos[i].x<< " ," <<pos[i].y<< " ," <<pos[i].z<<") "<<std::endl;
        }
        double dist = std::sqrt((pos[8].x-0.28)*(pos[8].x-0.28)+(pos[8].y+0.24)*(pos[8].y+0.24)+(pos[8].z-0.06)*(pos[8].z-0.06));
        if(dist > maxDist){
            maxDist = dist;
        }
        // std::cout << "link_pos 6"<< " : (" <<pos[5].x<< " ," <<pos[5].y<< " ," <<pos[5].z<<") "<<std::endl;
        std::cout << "max dist : "<<maxDist<<std::endl;
    }

private:
    ros::NodeHandle n_;
    ros::Publisher pub_;
    ros::Subscriber sub_;
    geometry_msgs::Point pos[9];
    geometry_msgs::Quaternion orientation[9];
    geometry_msgs::Vector3 vel[6];
    double maxDist = 0;
};

int main(int argc, char **argv)
{
    ros::init(argc, argv, "mission_5");

    ros::NodeHandle node_handle;
    ros::AsyncSpinner spinner(1);
    SubscribeAndPublish SAPObject;
    double begin;
    ros::Time t(0);
    do{
        ros::Time now = ros::Time::now();
        begin = now.toSec() - t.toSec();
    }while (begin == 0);
    
    begin = ros::Time::now().toSec();

    ROS_INFO_STREAM("start");
    ros::Publisher pos_pub = node_handle.advertise<std_msgs::Float64MultiArray>("/probot_anno/arm_vel_controller/command", 100);

    double end_pos[4][6] = {{0.2289, 0, 0.432, M_PI / 2, 0, 0},
                            {0.26, 0.15, 0.06, M_PI / 2, 0, 0},
                            {0.4, 0, 0.2, M_PI / 2, 0, 0},
                            {0.28, -0.24, 0.06, M_PI / 2, 0, 0}};
    std::vector<double> link[4];
    for (int i = 0; i < 4; i++)
    {
        link[i] = calIKPos(end_pos[i]);
    }

    double routePos[6][4];
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 6; j++)
        {
            routePos[j][i] = link[i][j];
        }
    }

    // double routePos[6][4] = {
    //     {0, 0.795499, 0.387524, -0.387524},
    //     {0, -0.772855, -1.07552, -1.29294},
    //     {0, 0.205023, -0.199728, -0.0654229},
    //     {0, -2.05585, -2.73831, 2.74605},
    //     {0, 0.631168, 0.273048, 0.196463},
    //     {0, 2.29983, 3.02704, -3.06027}};
    double routeVel[6][4] = {
        {0, 0, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0}};
    double routeAcc[6][4] = {
        {0, 0, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0}};
    double endT[3] = {5, 2.5, 2.5};

    std::vector<std::vector<double>> a;
    a.resize(6);
    double result_vel[6];
    ros::Rate r(10);

    std_msgs::Float64MultiArray target_pos;
    for (int i = 0; i < 6; i++)
    {
        target_pos.data.push_back(0);
    }

    for (int i = 0; i < 3; i++)
    {
        double duration = 0.1;
        for(int w = 0 ;w <endT[i]*10; w++)
        {
            
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
            for (int z = 0; z < 6; z++)
            {
                target_pos.data.at(z) = result_vel[z];
            }
            pos_pub.publish(target_pos);
            duration += 0.1;
            r.sleep();
            // std::cout << "times : " << duration << std::endl;
        }
    }
    // ros::spinOnce();

    for (int time = 0; time < 9; time++)
    {
        double end_pos_2[5][6] = {{0.28, -0.24, 0.06, M_PI / 2, 0, 0},
                                  {0.4, 0, 0.18, M_PI / 2, 0, 0},
                                  {0.26, 0.15, 0.06, M_PI / 2, 0, 0},
                                  {0.4, 0, 0.18, M_PI / 2, 0, 0},
                                  {0.28, -0.24, 0.06, M_PI / 2, 0, 0}};
        std::vector<double> link_2[5];
        for (int i = 0; i < 5; i++)
        {
            link_2[i] = calIKPos(end_pos_2[i]);
        }

        double routePos_2[6][5];
        for (int i = 0; i < 5; i++)
        {
            for (int j = 0; j < 6; j++)
            {
                routePos_2[j][i] = link_2[i][j];
            }
        }
        double routeVel_2[6][5] = {
            {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0}};
        double routeAcc_2[6][5] = {
            {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0}};
        double endT_2[4] = {2.5, 2.5, 2.5, 2.5};

        std::vector<std::vector<double>> a_2;
        a_2.resize(6);
        double result_vel_2[6];

        std_msgs::Float64MultiArray target_pos_2;
        for (int i = 0; i < 6; i++)
        {
            target_pos_2.data.push_back(0);
        }

        for (int i = 0; i < 4; i++)
        {
            double duration_2 = 0.1;

            for(int w=0; w<endT_2[i]*10 ; w++)
            {
                for (int j = 0; j < 6; j++)
                {
                    a_2[j] = calRoute(endT_2[i], routePos_2[j][i], routePos_2[j][i + 1],
                                      routeVel_2[j][i], routeVel_2[j][i + 1], routeAcc_2[j][i], routeAcc_2[j][i + 1]);
                }
                for (int k = 0; k < 6; k++)
                {
                    result_vel_2[k] = a_2[k][1] + 2 * a_2[k][2] * duration_2 + 3 * a_2[k][3] * duration_2 * duration_2 +
                                      4 * a_2[k][4] * duration_2 * duration_2 * duration_2 +
                                      5 * a_2[k][5] * duration_2 * duration_2 * duration_2 * duration_2;
                }
                for (int z = 0; z < 6; z++)
                {
                    target_pos_2.data.at(z) = result_vel_2[z];
                }
                pos_pub.publish(target_pos_2);
                duration_2 += 0.1;
                r.sleep();
                // std::cout << "times : " << duration_2 << std::endl;
            }
        }
        // ros::spinOnce();
        // for (int i = 0; i < 6; i++)
        // {
        //     target_pos_2.data.at(i) = 0;
        // }
        // pos_pub.publish(target_pos_2);
    }
    double times = ros::Time::now().toSec()-begin;

    std::cout<< times << std::endl;

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