#include <ros/ros.h>
#include <sensor_msgs/Image.h>
#include <opencv2/opencv.hpp>
#include <cv_bridge/cv_bridge.h>
// #include <image_transport/simple_publisher_plugin.h>
#include <iostream>
#include <vector>
#include "std_msgs/Float64MultiArray.h"
#include "squares.hpp"

// image_transport::ImageTransport my_publisher;
ros::Publisher my_publisher;
void myCallback(const sensor_msgs::ImageConstPtr &img)
{
    int g_nThresh = 30; //当前阀值
    int g_nMaxThresh = 175;
    cv::Mat imgimg;
    imgimg = cv_bridge::toCvShare(img, "bgr8")->image;

    //--------------------
    cv::Mat hsv_img, mask_img, green_img;
    green_img = Mat::zeros(imgimg.size(), imgimg.type());
    cv::cvtColor(imgimg, hsv_img, CV_BGR2HSV);
    inRange(hsv_img, Scalar(35, 0, 0), Scalar(77, 255, 255), mask_img);
    for (size_t r = 0; r < imgimg.rows; r++)
    {
        for (size_t c = 0; c < imgimg.cols; c++)
        {
            if (mask_img.at<uchar>(r,c) == 255)
            {
                green_img.at<Vec3b>(r,c)[0] = imgimg.at<Vec3b>(r,c)[0];
                green_img.at<Vec3b>(r,c)[1] = imgimg.at<Vec3b>(r,c)[1];
                green_img.at<Vec3b>(r,c)[2] = imgimg.at<Vec3b>(r,c)[2];
            }
        }
    }
    
    //--------------------
    cv::Mat gray_img, scharr_img, scharr_imgx, scharr_imgy, absscharr_imgx, absscharr_imgy;
    gray_img = cv::Mat::zeros(green_img.size(), CV_32FC1);
    cv::GaussianBlur(green_img, green_img, cv::Size(3, 3), 0, 0, cv::BORDER_DEFAULT);
    cv::cvtColor(green_img, gray_img, CV_BGR2GRAY);
    cv::Scharr(green_img, scharr_imgx, CV_16S, 1, 0, 1, 0, cv::BORDER_DEFAULT);
    cv::convertScaleAbs(scharr_imgx, absscharr_imgx);
    cv::Scharr(green_img, scharr_imgy, CV_16S, 0, 1, 1, 0, cv::BORDER_DEFAULT);
    cv::convertScaleAbs(scharr_imgy, absscharr_imgy);
    cv::addWeighted(absscharr_imgx, 0.5, absscharr_imgy, 0.5, 0, scharr_img);

    vector <Point> msg_img;
    std_msgs::Float64MultiArray msg;
    msg_img = detect_square(green_img);
    for (size_t i = 0; i < msg_img.size(); i++)
    {
        msg.data.push_back(msg_img[i].x);
        msg.data.push_back(msg_img[i].y);
    }
    my_publisher.publish(msg);

    // cv::imshow("imgimg", imgimg);
    // cv::imshow("mask_img", mask_img);
    // cv::imshow("green_img", green_img);
    // cv::imshow("scharr_img", scharr_img);
    // cv::waitKey(100);
}

int main(int argc, char *argv[])
{
    ros::init(argc, argv, "imgimgimage");
    ros::NodeHandle n;
    ros::Subscriber my_subscriber = n.subscribe("/probot_anno/camera/image_raw", 10, myCallback);
    my_publisher = n.advertise<std_msgs::Float64MultiArray>("/display_img", 10);
    ros::WallDuration(2).sleep();
    while (ros::ok())
    {
        ros::spinOnce();
    }
    return 0;
}