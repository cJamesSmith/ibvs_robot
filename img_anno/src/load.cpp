#include <ros/ros.h>
#include <sensor_msgs/Image.h>
#include <opencv2/opencv.hpp>
#include <cv_bridge/cv_bridge.h>
#include <iostream>

void myCallback(sensor_msgs::ImageConstPtr img)
{
    cv::Mat imgimg;
    imgimg = cv_bridge::toCvShare(img, "bgr8")->image;
    cv::namedWindow("sb", cv::WINDOW_AUTOSIZE);
    // ROS_INFO("%d %d", imgimg.size().height, imgimg.size().width);
    // std::cout << imgimg.channels();
    cv::imshow("sb", imgimg);
    // cv::imwrite("1.png", imgimg);
    cv::waitKey(250);
    // cv::destroyAllWindows();
}

int main(int argc, char *argv[])
{
    ros::init(argc, argv, "image_loader");
    ros::NodeHandle n;
    ros::Subscriber my_subscriber = n.subscribe("/probot_anno/camera/image_raw", 1, myCallback);
    ros::Rate naptime(2);
    while (ros::ok())
    {
        /* code */
        ros::spinOnce();
        naptime.sleep();
    }

    return 0;
}
