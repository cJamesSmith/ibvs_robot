#include <ros/ros.h>
#include <sensor_msgs/Image.h>
#include <opencv2/opencv.hpp>
#include <cv_bridge/cv_bridge.h>
#include <iostream>

void myCallback(const sensor_msgs::ImageConstPtr &img)
{
    int g_nThresh = 30; //当前阀值
    int g_nMaxThresh = 175;
    cv::Mat imgimg, gray_img;
    cv::Mat scharr_img, scharr_imgx, scharr_imgy, absscharr_imgx, absscharr_imgy;
    imgimg = cv_bridge::toCvShare(img, "bgr8")->image;
    gray_img = cv::Mat::zeros(imgimg.size(), CV_32FC1);
    cv::GaussianBlur(imgimg, imgimg, cv::Size(3, 3), 0, 0, cv::BORDER_DEFAULT);
    cv::cvtColor(imgimg, gray_img, CV_BGR2GRAY);
    cv::Scharr(imgimg, scharr_imgx, CV_16S, 1, 0, 1, 0, cv::BORDER_DEFAULT);
    cv::convertScaleAbs(scharr_imgx, absscharr_imgx);
    cv::Scharr(imgimg, scharr_imgy, CV_16S, 0, 1, 1, 0, cv::BORDER_DEFAULT);
    cv::convertScaleAbs(scharr_imgy, absscharr_imgy);
    cv::addWeighted(absscharr_imgx, 0.5, absscharr_imgy, 0.5, 0, scharr_img);
    //show
    cv::imshow("imgimg", imgimg);
    cv::imshow("scharr_img", scharr_img);
    cv::waitKey(100);
}

int main(int argc, char *argv[])
{
    ros::init(argc, argv, "display_image");
    ros::NodeHandle n;
    ros::Subscriber my_subscriber = n.subscribe("/probot_anno/camera/image_raw", 1, myCallback);
    while (ros::ok())
    {
        ros::spinOnce();
    }
    return 0;
}