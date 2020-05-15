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
double timeBegin;

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
        }
        double dist = std::sqrt((pos[5].x-0.2289)*(pos[5].x-0.2289)+(pos[5].y-0)*(pos[5].y-0)+(pos[5].z-0.454)*(pos[5].z-0.454));
        if(dist > maxDist){
            maxDist = dist;
        }
        std::cout << "max dist : "<<maxDist<<std::endl;
    }

private:
    ros::NodeHandle n_;
    ros::Publisher pub_;
    ros::Subscriber sub_;
    geometry_msgs::Point pos[6];
    geometry_msgs::Quaternion orientation[6];
    geometry_msgs::Vector3 vel[6];
    double maxDist = 0;
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

    do
    {
        ros::Time now_1 = ros::Time::now();
        timeBegin = now_1.toSec() - t.toSec();
    } while (timeBegin == 0);

    std_msgs::Float64MultiArray target_pos;
    for (int i = 0; i < 6; i++)
    {
        target_pos.data.push_back(0);
    }
    double end_pos[6] = {0};
    end_pos[0] = 0.2289;
    end_pos[1] = 0;
    end_pos[2] = 0.432;
    end_pos[3] = M_PI / 2;
    end_pos[4] = 0;
    end_pos[5] = 0;
    Eigen::Matrix<double, 6, 1> endVel;
    endVel << 0, 0, 0, 0.1046, -0.0785, 0;
    Eigen::Matrix<double, 6, 1> thetaVel;
    ros::Time now = ros::Time::now();
    double duration = (now.toSec() - t.toSec()) - timeBegin;
    // std::cout << "time : " << duration <<std::endl;
    while (duration <= 10)
    {
        ros::Time now_1 = ros::Time::now();
        duration = (now_1.toSec() - t.toSec()) - timeBegin;
        end_pos[4] = -0.0785 * duration;
        end_pos[3] = M_PI / 2 + 0.0785 * duration;
        std::vector<double> link;
        link.resize(6);
        link = calIKPos(end_pos);
        Eigen::Matrix<double, 6, 6> jacob;
        jacob = myCalJacob(link);
        thetaVel = jacob.inverse() * endVel;
        for (int i = 0; i < 6; i++)
        {
            target_pos.data.at(i) = thetaVel(i, 0);
        }
        pos_pub.publish(target_pos);
        std::cout << "time : " << duration << std::endl;
        ros::spinOnce();
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