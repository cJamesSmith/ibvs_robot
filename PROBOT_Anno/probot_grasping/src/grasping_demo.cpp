/***********************************************************************
Copyright 2019 Wuhan PS-Micro Technology Co., Itd.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
***********************************************************************/

#include "probot_grasping/grasping_demo.h"
#include <opencv2/opencv.hpp>
#include <sensor_msgs/image_encodings.h>
#include <pthread.h>
#include <iostream>


GraspingDemo::GraspingDemo(ros::NodeHandle n_, float pregrasp_x, float pregrasp_y, float pregrasp_z, float length, float breadth) : it_(n_),
                                                                                                                                    armgroup("manipulator"),
                                                                                                                                    grippergroup("gripper"),
                                                                                                                                    vMng_(length, breadth)
{
    this->nh_ = n_;

    try
    {
        this->tf_camera_to_robot.waitForTransform("/base_link", "/camera_link", ros::Time(0), ros::Duration(50.0));
    }
    catch (tf::TransformException &ex)
    {
        ROS_ERROR("[adventure_tf]: (wait) %s", ex.what());
        ros::Duration(1.0).sleep();
    }

    try
    {
        this->tf_camera_to_robot.lookupTransform("/base_link", "/camera_link", ros::Time(0), (this->camera_to_robot_));
    }

    catch (tf::TransformException &ex)
    {
        ROS_ERROR("[adventure_tf]: (lookup) %s", ex.what());
    }

    grasp_running = false;
    read_image_time = 1;
    img = cv::Mat::zeros(cv::Size(800, 800), CV_8UC1);

    this->pregrasp_x = pregrasp_x;
    this->pregrasp_y = pregrasp_y;
    this->pregrasp_z = pregrasp_z;

    ros::AsyncSpinner spinner(2);
    spinner.start();
    ros::WallDuration(0.5).sleep();
    // ROS_INFO_STREAM("Getting into the Grasping Position....");
    attainPosition(pregrasp_x, pregrasp_y, pregrasp_z);

    // Subscribe to input video feed and publish object location
    image_sub_ = it_.subscribe("/probot_anno/camera/image_raw", 1, &GraspingDemo::imageCb, this);
}

void GraspingDemo::imageCb(const sensor_msgs::ImageConstPtr &msg)
{
    if (true)
    {
        // ROS_INFO_STREAM("Processing the Image to locate the Object...");
        try
        {
            // if (read_image_time % 100 == 1)
            {
                img = cv_bridge::toCvCopy(msg, "bgr8")->image;
            }
            // std::cout << img.size() << std::endl;
            // ros::AsyncSpinner spinner(1);
            // spinner.start();
            // cv::imshow("sb", img);
            // cv::waitKey(10);
            // spinner.stop();
        }
        catch (cv_bridge::Exception &e)
        {
            ROS_ERROR("cv_bridge exception: %s", e.what());
            return;
        }
    }
#if 0
    // ROS_INFO("Image Message Received");
    float obj_x, obj_y;
    vMng_.get2DLocation(cv_ptr->image, obj_x, obj_y);

    // Temporary Debugging
    std::cout<< " X-Co-ordinate in Camera Frame :" << obj_x << std::endl;
    std::cout<< " Y-Co-ordinate in Camera Frame :" << obj_y << std::endl;

    obj_camera_frame.setZ(-obj_y);
    obj_camera_frame.setY(-obj_x);
    obj_camera_frame.setX(0.45);


	cv::Mat img;
	void* showImage(void*);
    obj_robot_frame = camera_to_robot_ * obj_camera_frame;
    grasp_running = true;

    // Temporary Debugging
    std::cout<< " X-Co-ordinate in Robot Frame :" << obj_robot_frame.getX() << std::endl;
    std::cout<< " Y-Co-ordinate in Robot Frame :" << obj_robot_frame.getY() << std::endl;
    std::cout<< " Z-Co-ordinate in Robot Frame :" << obj_robot_frame.getZ() << std::endl;
  }
#endif
}

void GraspingDemo::attainPosition(float x, float y, float z)
{
    namespace rvt = rviz_visual_tools;
    moveit_visual_tools::MoveItVisualTools visual_tools("base_link_2");
    visual_tools.deleteAllMarkers();
    geometry_msgs::PoseStamped currPose = armgroup.getCurrentPose();
    geometry_msgs::Pose target_pose1;
    target_pose1.orientation = currPose.pose.orientation;
    target_pose1.position.x = x;
    target_pose1.position.y = y;
    target_pose1.position.z = z;
    armgroup.setPoseTarget(target_pose1);
    armgroup.setMaxAccelerationScalingFactor(1);
    armgroup.setMaxVelocityScalingFactor(1);
    armgroup.setGoalPositionTolerance(0.01);
    armgroup.move();
}

void GraspingDemo::attainObject()
{
    // ROS_INFO("The attain Object function called");
    attainPosition(obj_robot_frame.getX(), obj_robot_frame.getY(), obj_robot_frame.getZ() + 0.04);

    // Open Gripper
    ros::WallDuration(1.0).sleep();
    grippergroup.setNamedTarget("open");
    grippergroup.move();

    // Slide down the Object
    geometry_msgs::PoseStamped currPose = armgroup.getCurrentPose();
    geometry_msgs::Pose target_pose1;

    target_pose1.orientation = currPose.pose.orientation;
    target_pose1.position = currPose.pose.position;

    target_pose1.position.z = obj_robot_frame.getZ() - 0.02;
    armgroup.setPoseTarget(target_pose1);
    armgroup.move();
}

void GraspingDemo::grasp()
{
    // ROS_INFO("The Grasping function called");

    ros::WallDuration(1.0).sleep();
    grippergroup.setNamedTarget("close");
    grippergroup.move();
}

void GraspingDemo::lift()
{
    // ROS_INFO("The lift function called");

    // For getting the pose
    geometry_msgs::PoseStamped currPose = armgroup.getCurrentPose();

    geometry_msgs::Pose target_pose1;
    target_pose1.orientation = currPose.pose.orientation;
    target_pose1.position = currPose.pose.position;

    // Starting Postion after picking
    //target_pose1.position.z = target_pose1.position.z + 0.06;

    if (rand() % 2)
    {
        target_pose1.position.y = target_pose1.position.y + 0.02;
    }
    else
    {
        target_pose1.position.y = target_pose1.position.y - 0.02;
    }

    armgroup.setPoseTarget(target_pose1);
    armgroup.move();

    // Open Gripper
    ros::WallDuration(1.0).sleep();
    grippergroup.setNamedTarget("open");
    grippergroup.move();

    target_pose1.position.z = target_pose1.position.z + 0.06;
    armgroup.setPoseTarget(target_pose1);
    armgroup.move();
}

void GraspingDemo::goHome()
{
    geometry_msgs::PoseStamped currPose = armgroup.getCurrentPose();

    // Go to Home Position
    // attainPosition(pregrasp_x, pregrasp_y, pregrasp_z);
    // attainPosition(homePose.pose.position.x, homePose.pose.position.y, homePose.pose.position.z);
    // attainPosition(0.2, -0.17, 0.2);
    // attainPosition(0.2, -0.17, 0.3);
    attainPosition(0.32, 0, 0.3);
    attainPosition(0.26, 0, 0.3);
}

void GraspingDemo::initiateGrasping()
{
    ros::AsyncSpinner spinner(1);
    spinner.start();
    ros::WallDuration(0.5).sleep();

    // homePose = armgroup.getCurrentPose();

    // ROS_INFO_STREAM("Approaching the Object....");
    // attainObject();

    // ROS_INFO_STREAM("Attempting to Grasp the Object now..");
    // grasp();

    // ROS_INFO_STREAM("Lifting the Object....");
    // lift();

    // ROS_INFO_STREAM("Going back to home position....");
    goHome();

    grasp_running = false;
}

void* showImage(void *imgg)
{
    while (true)
    {
        std::cout << imgg << std::endl;
        cv::imshow("sb", *((cv::Mat*)imgg));
        cv::waitKey(1);
    }
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "simple_grasping");
    float length, breadth, pregrasp_x, pregrasp_y, pregrasp_z;
    ros::NodeHandle n;

    if (!n.getParam("probot_grasping/table_length", length))
        length = 0.3;
    if (!n.getParam("probot_grasping/table_breadth", breadth))
        breadth = 0.3;
    if (!n.getParam("probot_grasping/pregrasp_x", pregrasp_x))
        pregrasp_x = 0.20;
    if (!n.getParam("probot_grasping/pregrasp_y", pregrasp_y))
        pregrasp_y = -0.17;
    if (!n.getParam("probot_grasping/pregrasp_z", pregrasp_z))
        pregrasp_z = 0.28;

    GraspingDemo simGrasp(n, pregrasp_x, pregrasp_y, pregrasp_z, length, breadth);

    while (ros::ok())
    {
        ros::spinOnce();
        simGrasp.initiateGrasping();
    }
    return 0;
}
